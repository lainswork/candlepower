
#include <iostream>
#include <tuple>
#include <unordered_map>
#include <workflow/Workflow.h>
#include <nlohmann/json.hpp>





#define CANDLEPOWER_NLOHMANN_DEFINE_TYPE_INTRUSIVE(Type, ...)  \
    friend void to_json(nlohmann::json& nlohmann_json_j, const Type& nlohmann_json_t) { NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_TO, __VA_ARGS__)) nlohmann_json_j["funname"] = #Type; } \
    friend void from_json(const nlohmann::json& nlohmann_json_j, Type& nlohmann_json_t) { NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_FROM, __VA_ARGS__)) nlohmann_json_t.funname = nlohmann_json_j["funname"]; }
namespace candlepower {

	using json_t = nlohmann::json;

	
	struct serverfun
	{
		std::string funname;
		virtual int execute(SeriesWork* series) = 0;
	};


	namespace funs {


		template <typename Function> 
		struct invoker {
			template <>
			static inline void apply(const Function& func, SeriesWork* series, const char* data,size_t size, std::string& result,) {
				using args_tuple = typename function_traits<Function>::args_tuple_2nd;
				
				msgpack_codec codec;
				try {
					auto tp = codec.unpack<args_tuple>(data, size);
					call(func, conn, result, std::move(tp));
					exe_model = model;
				}
				catch (std::invalid_argument& e) {
					result = codec.pack_args_str(result_code::FAIL, e.what());
				}
				catch (const std::exception& e) {
					result = codec.pack_args_str(result_code::FAIL, e.what());
				}
			}

			template <ExecMode model, typename Self>
			static inline void apply_member(const Function& func, Self* self,
				std::weak_ptr<connection> conn,
				const char* data, size_t size,
				std::string& result, ExecMode& exe_model) {
				using args_tuple = typename function_traits<Function>::args_tuple_2nd;
				exe_model = ExecMode::sync;
				msgpack_codec codec;
				try {
					auto tp = codec.unpack<args_tuple>(data, size);
					call_member(func, self, conn, result, std::move(tp));
					exe_model = model;
				}
				catch (std::invalid_argument& e) {
					result = codec.pack_args_str(result_code::FAIL, e.what());
				}
				catch (const std::exception& e) {
					result = codec.pack_args_str(result_code::FAIL, e.what());
				}
			}
		};








		





		struct router {
			std::unordered_map<
				std::string, std::function<bool(SeriesWork* series /*wf框架的任务流*/, json_t&/*参数json数组*/, size_t/*参数个数*/)>
			>
				map_invokers_;

			template <typename Function>
			void register_nonmember_func(std::string const& name, Function f) {
				this->map_invokers_[name] = { std::bind(
					&invoker<Function>::template apply<model>, std::move(f),
					std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
					std::placeholders::_4, std::placeholders::_5) };
			}


		};

		/*struct add : serverfun {
			int a;
			int b;
			virtual int execute(SeriesWork* series) override {
				return a + b;
			}
			CANDLEPOWER_NLOHMANN_DEFINE_TYPE_INTRUSIVE(add,a, b)

		};

		template<typename... Args>
		struct _fun
		{
			std::tuple<Args> args;
		};





		int add(int a ,int b) {
		
		}*/


	};



	



}





