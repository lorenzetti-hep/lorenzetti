
#ifndef StatusCode_h
#define StatusCode_h


enum class __ErrorCode__ { FAILURE = 0, SUCCESS = 1};

class StatusCode{
  public:
    StatusCode( __ErrorCode__ code ):m_code(code){;};
    ~StatusCode(){;};
    bool isSuccess() const { return (m_code==__ErrorCode__::SUCCESS) ? true : false; }
	  bool isFailure() const { return (m_code==__ErrorCode__::FAILURE) ? true : false; }
  private:
    __ErrorCode__ m_code;
};

namespace ErrorCode{
  // Provide shorthands for default code val ues
  const static auto SUCCESS = StatusCode( __ErrorCode__::SUCCESS );
  const static auto FAILURE = StatusCode( __ErrorCode__::FAILURE );
}

#endif
