
#include <iostream>
#include <tuple>
#include <unordered_map>
#include <workflow/Workflow.h>
#include <nlohmann/json.hpp>






namespace candlepower {

	namespace server {

		namespace rpc {

			template <typename T>
			struct function_traits;

			template <typename Ret, typename Arg, typename... Args>
			struct function_traits<Ret(Arg, Args...)> {
			public:
				enum { arity = sizeof...(Args) + 1 };
				typedef Ret function_type(Arg, Args...);
				typedef Ret return_type;
				using stl_function_type = std::function<function_type>;
				typedef Ret(*pointer)(Arg, Args...);

				typedef std::tuple<Arg, Args...> tuple_type;
				typedef std::tuple<std::remove_const_t<std::remove_reference_t<Arg>>, std::remove_const_t<std::remove_reference_t<Args>>...>
					bare_tuple_type;


				using  args_tuple = std::tuple<Args...>;
			};

			template <typename Ret>
			struct function_traits<Ret()> {
			public:
				enum { arity = 0 };
				typedef Ret function_type();
				typedef Ret return_type;
				using stl_function_type = std::function<function_type>;
				typedef Ret(*pointer)();

				typedef std::tuple<> tuple_type;
				typedef std::tuple<> bare_tuple_type;
				using args_tuple = std::tuple<std::string>;
				using args_tuple_2nd = std::tuple<std::string>;
			};

			template <typename Ret, typename... Args>
			struct function_traits<Ret(*)(Args...)> : function_traits<Ret(Args...)> {};

			template <typename Ret, typename... Args>
			struct function_traits<std::function<Ret(Args...)>>
				: function_traits<Ret(Args...)> {};

			template <typename ReturnType, typename ClassType, typename... Args>
			struct function_traits<ReturnType(ClassType::*)(Args...)>
				: function_traits<ReturnType(Args...)> {};

			template <typename ReturnType, typename ClassType, typename... Args>
			struct function_traits<ReturnType(ClassType::*)(Args...) const>
				: function_traits<ReturnType(Args...)> {};

			template <typename Callable>
			struct function_traits : function_traits<decltype(&Callable::operator())> {};


			template<typename... Args>
			constexpr size_t tuple_get_num(const std::tuple<Args...>& t) {
				return sizeof...(Args);
			}


			template<typename Tuple, size_t N>
			struct tuple_type_print
			{
				static std::string print(Tuple* t)
				{
					return tuple_type_print<Tuple, N - 1>::print(t) + ", " + fmt::format("{:s}", typeid(decltype(std::get<N - 1>(*t))).name());

				}
			};

			// 偏特性，可以理解为递归的终止
			template<typename Tuple>
			struct tuple_type_print< Tuple, 1>
			{
				static std::string print(Tuple* t)
				{
					return fmt::format("{:s}", typeid(decltype(std::get<0>(*t))).name());
				}
			};

			template<typename Tuple>
			struct tuple_type_print< Tuple, 0>
			{
				static std::string print(Tuple* t)
				{
					return "no parameters";
					//printf("This function has no parameters..\n");
				}
			};

			namespace argpack {
				using string_t = std::string;
				using json_t = nlohmann::json;



				//使用json数组打包参数
				template <typename... Args>
				static string_t pack(Args &&...args) {
					json_t json = std::forward_as_tuple(std::forward<Args>(args)...);
					return json.dump();
				}

				//解包参数
				template <typename T>
				static T unpack(std::string str) {
					T ret{};

					try {
						ret = json_t::parse(str);
					}
					catch (...) {
						spdlog::warn("args unpack erro! The required parameter list is: <{:s}> \tbut json array is:{:s}",
							tuple_type_print<decltype(ret), tuple_get_num(ret)>::print(&ret), str);
					}
					return ret;
				}
			}

			struct router {
				using connect_context_t = void*;
				using pdata_t = const char*;

				std::unordered_map<std::string, std::function<void(connect_context_t, pdata_t, size_t, std::string&)>>map_invokers_;


			public:

				template <typename _TFun>
				void register_handler(std::string const& name, _TFun f) {
					return register_nonmember_func(name, std::move(f));
				}

				template <typename _TFun, typename _TSelf>
				void register_handler(const std::string& name, const _TFun& f,
					_TSelf* self) {
					return register_member_func(name, f, self);
				}

				void remove_handler(std::string const& name) {
					this->map_invokers_.erase(name);
				}

				// 执行路由
				template <typename T>
				void route(connect_context_t conn,const std::string& funname, const std::string& jsonargs, std::string& result) {
					
					auto it = map_invokers_.find(funname);
					if (it == map_invokers_.end()) {
						return;
					}

					try
					{
						it->second(conn, jsonargs.c_str(), jsonargs.size(), result);
					}
					catch (...)
					{
						spdlog::warn("rpc function execution failed! function name: [{:s}]  args:{:s} ", funname, jsonargs);
					}
					
				}

			private:
				template <typename F, size_t... I, typename... Args>
				static typename std::result_of<F(connect_context_t, Args...)>::type call_helper(const F& f, const std::index_sequence<I...>&, std::tuple<Args...> tup, connect_context_t ptr) {
					return f(ptr, std::move(std::get<I>(tup))...);
				}


				//返回值为void
				template <typename F, typename... Args>
				static typename std::enable_if<std::is_void<typename std::result_of<F(connect_context_t, Args...)>::type>::value>::type call(const F& f, connect_context_t ptr, std::string& result, std::tuple< Args...> tp) {

					call_helper(f, std::make_index_sequence<sizeof...(Args)>{}, std::move(tp), ptr);

				}

				//返回值不为void
				template <typename F, typename... Args>
				static typename std::enable_if<!std::is_void<typename std::result_of<F(connect_context_t, Args...)>::type>::value>::type call(const F& f, connect_context_t ptr, std::string& result, std::tuple<Args...> tp) {
					auto r = call_helper(f, std::make_index_sequence<sizeof...(Args)>{}, std::move(tp), ptr);
					result = argpack::pack(r);
				}


				template <typename F, typename Self, size_t... Indexes, typename... Args>
				static typename std::result_of<F(Self, connect_context_t, Args...)>::type call_member_helper(const F& f, Self* self, const std::index_sequence<Indexes...>&, std::tuple< Args...> tup, connect_context_t ptr) {
					return (*self.*f)(ptr, std::move(std::get<Indexes>(tup))...);
				}

				template <typename F, typename Self, typename... Args>
				static typename std::enable_if<std::is_void<typename std::result_of<F(Self, connect_context_t, Args...)>::type>::value>::type call_member(const F& f, Self* self, connect_context_t ptr, std::string& result, std::tuple<Args...> tp) {
					call_member_helper(f, self, typename std::make_index_sequence<sizeof...(Args)>{}, std::move(tp), ptr);
				}

				template <typename F, typename Self, typename... Args>
				static typename std::enable_if<!std::is_void<typename std::result_of<F(Self, connect_context_t, Args...)>::type>::value>::type call_member(const F& f, Self* self, connect_context_t ptr, std::string& result, std::tuple<Args...> tp) {
					auto r = call_member_helper(f, self, typename std::make_index_sequence<sizeof...(Args)>{}, std::move(tp), ptr);
					result = argpack::pack(r);
				}


				template <typename Function>
				struct invoker {
					
					static inline void apply(const Function& func, void* conn, pdata_t data, size_t size, std::string& result) {
						using args_tuple = typename function_traits<Function>::args_tuple;
						args_tuple tp = argpack::unpack<args_tuple>(std::string(data, size));
						call(func, conn, result, std::move(tp));
					}


					template <typename Self>
					static inline void apply_member(const Function& func, Self* self, connect_context_t conn, pdata_t data, size_t size, std::string& result) {
						using args_tuple = typename function_traits<Function>::args_tuple;
						args_tuple tp = argpack::unpack<args_tuple>(std::string(data, size));
						call_member(func, self, conn, result, std::move(tp));
					}

				};


				// 注册一个普通函数
				//
				template <typename _TFun>
				void register_nonmember_func(const std::string& fun_name, _TFun fun) {

					map_invokers_[fun_name] =
					{ std::bind(&invoker<_TFun>::apply, fun,
						std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
						std::placeholders::_4) };
				}

				// 注册一个类成员函数
				template <typename _TFun, typename Self>
				void register_member_func(const std::string& fun_name, const _TFun& fun, Self* self) {
					this->map_invokers_[fun_name] =
					{ std::bind(
						&invoker<_TFun>::template apply_member<Self>, fun, self,
						std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
						std::placeholders::_4) };
				}


			};

		} // namespace rpc_service


	};

	



}





