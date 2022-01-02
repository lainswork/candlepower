namespace JsBind
{


	template< size_t... _Indexes >
	struct X_Index_tuple
	{

	};

	/// Builds an X_Index_tuple< 0, 1, 2, ..., _Num - 1 >.
	template< std::size_t _Num, typename _Tuple = X_Index_tuple<> >
	struct X_Build_index_tuple;

	template< std::size_t _Num, size_t... _Indexes >
	struct X_Build_index_tuple<_Num, X_Index_tuple< _Indexes... > >
		: X_Build_index_tuple< _Num - 1, X_Index_tuple< _Indexes..., sizeof...(_Indexes) > >
	{

	};

	template< size_t... _Indexes >
	struct X_Build_index_tuple< 0, X_Index_tuple< _Indexes... > >
	{
		typedef X_Index_tuple< _Indexes... > __type;
	};

	template<typename T>
	struct JsValAutoConv {};

	template<>
	struct JsValAutoConv<std::string>
	{
		std::pair<bool, std::string> Conv(JSContext* cx, jsval vp)
		{
			//jsval jval{};
			//if (JS_ConvertValue(cx, vp, JSTYPE_STRING, &jval) == JS_TRUE) {

			//	auto str = JS_ValueToString(cx, jval);


			//	if (!str)
			//		return std::make_pair(false, "");
			//	return std::make_pair(true, stl::wchar2char(str->getChars(cx)).c_str());
			//}

			//return std::make_pair(false, "");
			auto str = js_ValueToString(cx, js::Valueify(vp));

			if (!str)
				return std::make_pair(false, "");
			return std::make_pair(true, JsUnicodeToAnsi(str->getChars(cx)));


		}

	};


	template<>
	struct JsValAutoConv<std::wstring>
	{
		std::pair<bool, std::wstring> Conv(JSContext* cx, jsval vp)
		{
			/*jsval jval{};
			if (JS_ConvertValue(cx, vp, JSTYPE_STRING, &jval) == JS_TRUE) {
				auto str = JS_ValueToString(cx, jval);
				if (!str)
					return std::make_pair(false, L"");
				return std::make_pair(true, str->getChars(cx));
			}
			return std::make_pair(false, L"");*/

			auto str = js_ValueToString(cx, js::Valueify(vp));

			if (!str)
				return std::make_pair(false, L"");
			return std::make_pair(true, str->getChars(cx));

		}

	};

#ifdef __OBJ_BASE_STRING_HEARD_HPP__

	template<>
	struct JsValAutoConv<stl::string>
	{
		std::pair<bool, stl::string> Conv(JSContext* cx, jsval vp)
		{
			/*jsval jval{};
			if (JS_ConvertValue(cx, vp, JSTYPE_STRING, &jval) == JS_TRUE) {

				auto str = JS_ValueToString(cx, jval);
				if (!str)
					return std::make_pair(false, "");
				return std::make_pair(true, stl::wchar2char(str->getChars(cx)));
			}

			return std::make_pair(false, "");*/

			auto str = js_ValueToString(cx, js::Valueify(vp));

			if (!str)
				return std::make_pair(false, "");
			auto ansi_str = JsUnicodeToAnsi(str->getChars(cx));
			return std::make_pair(true, ansi_str.c_str());

		}

	};


	template<>
	struct JsValAutoConv<stl::wstring>
	{
		std::pair<bool, stl::wstring> Conv(JSContext* cx, jsval vp)
		{

			/*jsval jval{};
			if (JS_ConvertValue(cx, vp, JSTYPE_STRING, &jval) == JS_TRUE) {
				auto str = JS_ValueToString(cx, jval);
				if (!str)
					return std::make_pair(false, L"");
				return std::make_pair(true, str->getChars(cx));
			}
			return std::make_pair(false, L"");*/

			auto str = js_ValueToString(cx, js::Valueify(vp));

			if (!str)
				return std::make_pair(false, L"");


			return std::make_pair(true, str->getChars(cx));

		}

	};
#endif

	template<>
	struct JsValAutoConv<int>
	{
		std::pair<bool, int> Conv(JSContext* cx, jsval vp)
		{
			/*jsval jval{};
			if (JS_ConvertValue(cx, vp, JSTYPE_NUMBER, &jval) == JS_TRUE) {
				int32 ip = 0;
				JS_ValueToInt32(cx, jval, &ip);
				return std::make_pair(true, ip);
			}


			return std::make_pair(false, 0);*/

			int32 ip = 0;
			if (JS_ValueToInt32(cx, vp, &ip) == JS_TRUE)
				return std::make_pair(true, ip);


			return std::make_pair(false, 0);
		}
	};
	template<>
	struct JsValAutoConv<double>
	{
		std::pair<bool, int> Conv(JSContext* cx, jsval vp)
		{

			/*jsval jval = {};
			if (JS_ConvertValue(cx, vp, JSTYPE_NUMBER, &jval) == JS_TRUE) {
				jsdouble dp = 0.f;
				JS_ValueToNumber(cx, jval, &dp);
				return std::make_pair(true, dp);
			}

			return std::make_pair(false, 0);*/

			jsdouble dp = 0.f;
			if (JS_ValueToNumber(cx, vp, &dp) == JS_TRUE)
			{
				return std::make_pair(true, dp);
			}
			return std::make_pair(false, 0);
		}
	};
	template<>
	struct JsValAutoConv<bool>
	{
		std::pair<bool, bool> Conv(JSContext* cx, jsval vp)
		{

			/*jsval jval;
			if (JS_ConvertValue(cx, vp, JSTYPE_NUMBER, &jval) == JS_TRUE) {
				JSBool bp = false;
				JS_ValueToBoolean(cx, jval, &bp);
				return std::make_pair(true, (bool)bp);
			}

			return std::make_pair(false, false);*/

			JSBool bp = false;
			if (JS_ValueToBoolean(cx, vp, &bp) == JS_TRUE)
			{
				return std::make_pair(true, (bool)bp);
			}
			return std::make_pair(false, false);
		}
	};


	template<>
	struct JsValAutoConv<JSObject*>
	{
		std::pair<bool, JSObject*> Conv(JSContext* cx, jsval vp)
		{
			/*JSObject* bp = 0;
			jsval jval;
			if (JS_ConvertValue(cx, vp, JSTYPE_OBJECT, &jval) == JS_TRUE) {
				JS_ValueToObject(cx, jval, &bp);
				return std::make_pair(true, bp);
			}
			return std::make_pair(false, bp);*/
			JSObject* bp = 0;
			if (JS_ValueToObject(cx, vp, &bp) == JS_TRUE)
			{
				return std::make_pair(true, bp);
			}
			return std::make_pair(false, bp);

		}
	};


	template<typename T>
	struct AutoSetJsRet {};

	template<>
	struct AutoSetJsRet<std::string>
	{
		JSBool Conv(JSContext* cx, jsval* vp, std::string data)
		{
			auto unicode_str = JsAnsiToUnicode(data);
			JSString* jstr = JS_NewUCStringCopyZ(cx, unicode_str.c_str());
			jsval jv = STRING_TO_JSVAL(jstr);
			JS_SET_RVAL(cx, vp, jv);
			return JS_TRUE;

			//JSString* jstr = JS_NewStringCopyZ(cx, data.c_str());
			//jsval jv = STRING_TO_JSVAL(jstr);
			//JS_SET_RVAL(cx, vp, jv);
			//return JS_TRUE;

		}

	};

	template<>
	struct AutoSetJsRet<char*>
	{
		JSBool Conv(JSContext* cx, jsval* vp, char* data)
		{
			auto unicode_str = JsAnsiToUnicode(data);
			JSString* jstr = JS_NewUCStringCopyZ(cx, unicode_str.c_str());
			jsval jv = STRING_TO_JSVAL(jstr);
			JS_SET_RVAL(cx, vp, jv);
			return JS_TRUE;

			//JSString* jstr = JS_NewStringCopyZ(cx, data.c_str());
			//jsval jv = STRING_TO_JSVAL(jstr);
			//JS_SET_RVAL(cx, vp, jv);
			//return JS_TRUE;

		}

	};

	template<>
	struct AutoSetJsRet<std::wstring>
	{
		JSBool Conv(JSContext* cx, jsval* vp, std::wstring data)
		{
			JSString* jstr = JS_NewUCStringCopyZ(cx, data.c_str());
			jsval jv = STRING_TO_JSVAL(jstr);
			JS_SET_RVAL(cx, vp, jv);
			return JS_TRUE;
		}

	};

#ifdef __OBJ_BASE_STRING_HEARD_HPP__


	template<>
	struct AutoSetJsRet<stl::string>
	{
		JSBool Conv(JSContext* cx, jsval* vp, stl::string data)
		{
			auto unicode_str = JsAnsiToUnicode(data.c_str());
			JSString* jstr = JS_NewUCStringCopyZ(cx, unicode_str.c_str());
			jsval jv = STRING_TO_JSVAL(jstr);
			JS_SET_RVAL(cx, vp, jv);
			return JS_TRUE;

			//JSString* jstr = JS_NewStringCopyZ(cx, data.c_str());
			//jsval jv = STRING_TO_JSVAL(jstr);
			//JS_SET_RVAL(cx, vp, jv);
			//return JS_TRUE;
		}

	};
	template<>
	struct AutoSetJsRet<stl::wstring>
	{
		JSBool Conv(JSContext* cx, jsval* vp, stl::wstring data)
		{
			auto ansi_str = stl::wchar2char(data);
			JSString* jstr = JS_NewStringCopyZ(cx, ansi_str.c_str());
			jsval jv = STRING_TO_JSVAL(jstr);
			JS_SET_RVAL(cx, vp, jv);
			return JS_TRUE;
		}

	};
#endif

	template<>
	struct AutoSetJsRet<int>
	{
		JSBool Conv(JSContext* cx, jsval* vp, int data)
		{
			jsval jv = INT_TO_JSVAL(data);
			JS_SET_RVAL(cx, vp, jv);
			return JS_TRUE;
		}
	};

	template<>
	struct AutoSetJsRet<unsigned int>
	{
		JSBool Conv(JSContext* cx, jsval* vp, int data)
		{
			jsval jv = UINT_TO_JSVAL(data);
			JS_SET_RVAL(cx, vp, jv);
			return JS_TRUE;
		}
	};

	template<>
	struct AutoSetJsRet<double>
	{
		JSBool Conv(JSContext* cx, jsval* vp, double data)
		{
			jsval jv = DOUBLE_TO_JSVAL(data);
			JS_SET_RVAL(cx, vp, jv);
			return JS_TRUE;
		}
	};
	template<>
	struct AutoSetJsRet<bool>
	{
		JSBool Conv(JSContext* cx, jsval* vp, bool data)
		{

			jsval jv = BOOLEAN_TO_JSVAL(data);
			JS_SET_RVAL(cx, vp, jv);
			return JS_TRUE;

		}
	};



	template<>
	struct AutoSetJsRet<JSObject*>
	{
		JSBool Conv(JSContext* cx, jsval* vp, JSObject* data)
		{

			jsval jv = OBJECT_TO_JSVAL(data);
			JS_SET_RVAL(cx, vp, jv);
			return JS_TRUE;

		}
	};


	template<typename, typename>
	struct JsCallBind;

	template<typename one_call, typename Ret, typename ...Args>
	struct JsCallBind<one_call, Ret(Args...)>
	{
		using X_Tuple = std::tuple< Args...>;
		using X_Indices = typename X_Build_index_tuple< std::tuple_size< X_Tuple >::value >::__type;


		template<typename T>
		struct _S_Invoke
		{

			template< size_t... _Ind >
			static JSBool call(JSContext* cx, jsval* vp, X_Tuple&& xtuple, X_Index_tuple< _Ind... >)
			{
				// 解包 xtuple 参数，传递给 _call() 函数调用
				auto result = _call(std::get< _Ind >(std::move(xtuple))...);
				return AutoSetJsRet<std::remove_const_t<std::remove_reference_t<Ret>>>{}.Conv(cx, vp, result);
			}

		};


		template<>
		struct _S_Invoke<void>
		{
			template< size_t... _Ind >
			static JSBool call(JSContext* cx, jsval* vp, X_Tuple&& xtuple, X_Index_tuple< _Ind... >)
			{
				// 解包 xtuple 参数，传递给 _call() 函数调用
				_call(std::get< _Ind >(std::move(xtuple))...);
				JS_SET_RVAL(cx, vp, JSVAL_VOID);
				return JS_TRUE;
			}
		};


		template<typename Tuple, size_t N>
		struct tuple_set
		{
			static void set(Tuple* t, jsval* vp, JSContext* cx, bool& status)
			{
				if (!status)
				{
					return;
				}

				tuple_set<Tuple, N - 1>::set(t, vp, cx, status);
				auto result = JsValAutoConv<std::remove_const_t<std::remove_reference_t<decltype(std::get<N - 1>(*t))>>>{}.Conv(cx, vp[N - 1]);

				if (result.first)
				{
					std::get<N - 1>(*t) = result.second;
				}
				else
				{
					printf("arg%d conv %s fail...\n", 0, typeid(decltype(std::get<N - 1>(*t))).name());
					status = false;
				}
			}
		};

		// 偏特性，可以理解为递归的终止
		template<typename Tuple>
		struct tuple_set<Tuple, 1>
		{
			static void set(Tuple* t, jsval* vp, JSContext* cx, bool& status)
			{
				if (!status)
				{
					return;
				}

				auto result = JsValAutoConv<std::remove_const_t<std::remove_reference_t<decltype(std::get<0>(*t))>>>{}.Conv(cx, vp[0]);
				if (result.first)
				{
					std::get<0>(*t) = result.second;
				}
				else
				{
					printf("arg%d conv %s fail...\n", 0, typeid(decltype(std::get<0>(*t))).name());
					status = false;
				}
			}
		};

		template<typename Tuple>
		struct tuple_set<Tuple, 0>
		{
			static void set(Tuple* t, jsval* vp, JSContext* cx, bool& status)
			{

			}
		};



		template<typename Tuple, size_t N>
		struct tuple_type_print
		{
			static void print(Tuple* t)
			{
				tuple_type_print<Tuple, N - 1>::print(t);
				printf("arg%d type: %s \n", int(N - 1), typeid(decltype(std::get<N - 1>(*t))).name());
			}
		};

		// 偏特性，可以理解为递归的终止
		template<typename Tuple>
		struct tuple_type_print< Tuple, 1>
		{
			static void print(Tuple* t)
			{

				printf("arg%d type: %s \n", 0, typeid(decltype(std::get<0>(*t))).name());
			}
		};

		template<typename Tuple>
		struct tuple_type_print< Tuple, 0>
		{
			static void print(Tuple* t)
			{
				printf("This function has no parameters..\n");
			}
		};



		static JSBool jscall(JSContext* cx, uintN argc, jsval* vp)
		{
			auto nargs = sizeof...(Args);

			X_Tuple xtuple;

			if (argc != nargs)
			{
				printf("Fail! The number of parameters is inconsistent\n");
				tuple_type_print<decltype(xtuple), sizeof...(Args)>::print(&xtuple);
				return JS_FALSE;
			}

			jsval* argv = JS_ARGV(cx, vp);
			bool status = true;
			tuple_set<decltype(xtuple), sizeof...(Args)>::set(&xtuple, argv, cx, status);

			JSBool result{};

			if (status)
			{
				result = _S_Invoke<Ret>::call(cx, vp, std::forward< X_Tuple >(xtuple), X_Indices());
			}
			else
			{
				tuple_type_print<decltype(xtuple), sizeof...(Args)>::print(&xtuple);
			}

			return result;
		}



		JSFunctionSpec bind(std::string name, std::function<Ret(Args...)>&& func)
		{

			_name = name;
			_call = std::forward<std::function<Ret(Args...)>>(func);
			JSFunctionSpec js_function_spec;
			js_function_spec.name = _name.c_str();
			js_function_spec.flags = JSPROP_READONLY | JSPROP_PERMANENT;
			js_function_spec.nargs = sizeof...(Args);
			js_function_spec.call = jscall;
			return js_function_spec;
		}



		inline static std::function < Ret(Args...) > _call{};
		inline static std::string _name{};
	};


}