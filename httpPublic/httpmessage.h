#ifndef ___HTTP__MESSAGE__H___
#define ___HTTP__MESSAGE__H___

#include <map>
#include <string>

namespace unm
{
	namespace http
	{

		const std::string HTTP_METHOD_GET		= "GET";
		const std::string HTTP_METHOD_POST		= "POST";
		const std::string HTTP_METHOD_PUT		= "PUT";
		const std::string HTTP_METHOD_DELETE	= "DELETE";


		const std::string HTTP_HEADER_CONNECTION		= "Connection";
		const std::string HTTP_HEADER_CONTENT_LENGHT   = "Content-Length";
		const std::string HTTP_HEADER_CONTENT_TYPE     = "Content-Type";
		const std::string HTTP_HEADER_USER_AGENT	   = "User-Agent";
		const std::string HTTP_HEADER_HOST			   = "Host";

//////////////////////////////////////////////////////////////////////////
		class CHttpMessage
		{
		public:
			CHttpMessage();
			virtual ~CHttpMessage();

		public:
			virtual bool IsRequest() const = 0;
			virtual std::string GetHeader() const;
			virtual std::string GetPacket() const = 0;

			bool DoesHeaderFieldExist(const std::string &strField) const;
			std::string GetHeaderField(const std::string &strField) const;
			std::string GetBody() const;

			void RemoveHeaderField(const std::string &strField);
			void SetHeaderField(const std::string &strField, const std::string &strValue);
			void SetBody(const std::string &strBody, const std::string &strContentType);

		protected:
			std::map<std::string, std::string>								m_mapHeaders;
			std::string														m_strBody;
		};
	}
}

#endif
