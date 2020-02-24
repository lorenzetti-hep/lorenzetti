
#ifndef StatusCode_h
#define StatusCode_h

class StatusCode{

  enum class ErrorCode { FAILURE = 0, SUCCESS = 1};

  /**
   * @class StatusCode::Category
   *
   * The category assigned to a StatusCode. Derive from this class to implement your own category.
   * The mapping of StatusCode values to success and recoverable conditions can be defined by
   * overriding the appropriate methods.
   */
  class Category {

		private:
			ErrorCode m_code;
		
		public:
			void Category( ErroCode code):m_code(c){;};

    	/// Is code considered success ?
    	/// \note isFailure() cannot be overridden as it is defined as `!`isSuccess()
    	bool isSuccess() const { return (code==ErroCode::SUCCESS) ? true : false; }

			bool isFailure() const { return (code==ErrorCode::FAILURE) ? true : false; }

  };

  // Provide shorthands for default code values
  const static auto SUCCESS     = Category( ErrorCode::SUCCESS );
  const static auto FAILURE     = Category( ErrorCode::FAILURE );

};

#endif
