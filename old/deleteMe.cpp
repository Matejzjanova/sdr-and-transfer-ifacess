enum class codes{ok, notOk, fail,fail1}

codes foo(....)
	...

	...
	return codes::notOk;
	..
	....

return codes::ok;
}


void codeHandler(codes code){

	switch(code)
	case codes::ok:
		return;
	case codes::notOk:
		throw std::runtime_error("notOk);
	case codes::someCode*:
		throw std::runtime_error("someCode*")

}

void fooWrapper(...){

	auto res = foo(...);
	codeHendler(res);
	

}

