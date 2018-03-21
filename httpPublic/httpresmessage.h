#ifndef __HTTP__RES__MESSAGE__INCLUDE__
#define __HTTP__RES__MESSAGE__INCLUDE__

#include "httpmessage.h"

namespace unm
{
	namespace http
	{

		class CHttpResMessage : public CHttpMessage
		{
		public:
			CHttpResMessage();
			virtual ~CHttpResMessage();

		public:
			bool SetMessage(const std::string &strHeader, const std::string &strBody);
			virtual bool IsRequest() const
			{
				return false;
			}
			virtual std::string GetHeader() const;
			virtual std::string GetPacket() const;

			void SetCode(int nCode, const std::string &strDesc, const std::string &strProtocol = "HTTP/1.1");
			int GetCode() const;

			std::string GetCodeDesc() const;
			std::string GetProtocol() const;

		private:
			int										m_nCode;
			std::string								m_strCodeDesc;
			std::string								m_strProtocol;
		};
	}
}

#endif
