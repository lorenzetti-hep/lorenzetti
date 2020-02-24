
#ifndef StatusCode_h
#define StatusCode_h



enum class ErrorCode { FAILURE = 0, SUCCESS = 1};

class StatusCode{
  public:
    StatusCode( ErrorCode code ):m_code(code){;};
    ~StatusCode(){;};
    bool isSuccess() const { return (m_code==ErrorCode::SUCCESS) ? true : false; }
	  bool isFailure() const { return (m_code==ErrorCode::FAILURE) ? true : false; }
  private:
    ErrorCode m_code;
};

// Provide shorthands for default code values
const static auto SUCCESS = StatusCode( ErrorCode::SUCCESS );
const static auto FAILURE = StatusCode( ErrorCode::FAILURE );


#endif
