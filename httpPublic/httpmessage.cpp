#include "httpmessage.h"
#include <sstream>

namespace unm
{
	namespace http
	{
		CHttpMessage::CHttpMessage()
		{
		}

		CHttpMessage::~CHttpMessage()
		{
		}

		std::string CHttpMessage::GetHeader() const
		{
			std::ostringstream oss;
			std::map<std::string, std::string>::const_iterator it = m_mapHeaders.begin();
			for (; it != m_mapHeaders.end(); ++ it)
			{
				oss << it->first << ": " << it->second << "\r\n";
			}

			return oss.str();
		}

		bool CHttpMessage::DoesHeaderFieldExist(const std::string &strField) const
		{
			return (m_mapHeaders.find(strField) != m_mapHeaders.end());
		}

		std::string CHttpMessage::GetHeaderField(const std::string &strField) const
		{
			std::map<std::string, std::string>::const_iterator it = m_mapHeaders.find(strField);
			if (it != m_mapHeaders.end())
			{
				return it->second;
			}

			return "";
		}

		std::string CHttpMessage::GetBody() const
		{
			return m_strBody;
		}

		void CHttpMessage::RemoveHeaderField(const std::string &strField)
		{
			m_mapHeaders.erase(strField);
		}

		void CHttpMessage::SetHeaderField(const std::string &strField, const std::string &strValue)
		{
			m_mapHeaders[strField] = strValue;
		}

		void CHttpMessage::SetBody(const std::string &strBody, const std::string &strContentType)
		{
			m_strBody = strBody;
			m_mapHeaders[HTTP_HEADER_CONTENT_TYPE] = strContentType;

			char szLen[32];
			sprintf(szLen, "%ld", (long)strBody.length());
			m_mapHeaders[HTTP_HEADER_CONTENT_LENGHT] = szLen;
		}
	}
}
