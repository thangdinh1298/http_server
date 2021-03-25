#pragma once

class HTTPResponse {
public:
	HTTPResponse() = delete
	HTTPResponse(FILE* body) : body_(body) {}
	size_t write_to_body(const char* buf, size_t len) {
		return fwrite(buf, sizeof(char), len, body_);
	}
	void set_code(int code) {
		code_ = code;
	}
	void set_msg(std::string msg) {
		msg_ = msg;
	}
private:
	std::string msg_="OK";
	int code_=200;
	std::string version_="HTTP/1.0";
	FILE* body_;
};
