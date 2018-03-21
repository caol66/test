#include "httpresmessage.h"
#include <sstream>
#include <vector>
#include <stdlib.h>

#include "unmutility/util/StringUtil.h"

namespace unm
{
	namespace http
	{
		CHttpResMessage::CHttpResMessage()
		{
		}

		CHttpResMessage::~CHttpResMessage()
		{
		}

		bool CHttpResMessage::SetMessage(const std::string &strHeader, const std::string &strBody)
		{
			bool bSuccess = false;

			do
			{
				m_nCode = 0;
				m_strCodeDesc.clear();
				m_strProtocol.clear();
				m_mapHeaders.clear();

				std::vector<std::string> vecLine;
				unm::StringUtil::Split(vecLine,strHeader,"\r\n");
				if (vecLine.empty())
				{
					break;
				}

				// 取出请求行
				std::string strReqLine = vecLine[0];

				// 请求行样例: HTTP/1.1 206 Partial Content
				std::vector<std::string> vecReqLine;
				unm::StringUtil::Split(vecReqLine,strReqLine," ");
				if (vecReqLine.size() < 3)
				{
					break;
				}

				m_strProtocol = vecReqLine.at(0);
				m_nCode = unm::StringUtil::ToInt(vecReqLine.at(1));

				// 从原串查找第二个空格
				std::string::size_type nPos = strReqLine.find(' ');
				nPos = strReqLine.find(' ', nPos + 1);
				++nPos;
				m_strCodeDesc = strReqLine.c_str() + nPos;

				int nVecSize = vecLine.size();
				for (size_t i = 1; i < nVecSize; ++i)
				{
					const std::string &str = vecLine[i];
					std::string::size_type nPos = str.find(':');
					if (nPos != std::string::npos)
					{
						std::string strKey(str.c_str(), nPos);
						if (nPos + 1 < str.length() && str.at(nPos + 1) == ' ')
						{
							++nPos;
						}
						SetHeaderField(strKey, str.c_str() + nPos + 1);
					}
				}

				if (!strBody.empty())
				{
					if (!DoesHeaderFieldExist("Content-Type"))
					{
						break;
					}

					SetBody(strBody, GetHeaderField("Content-Type"));
				}

				bSuccess = true;
			}
			while (0);

			return bSuccess;
		}

		std::string CHttpResMessage::GetHeader() const
		{
			std::ostringstream oss;
			oss << m_strProtocol << " " << m_nCode << " " << m_strCodeDesc << "\r\n"
				<< CHttpMessage::GetHeader();

			return oss.str();
		}

		std::string CHttpResMessage::GetPacket() const
		{
			std::ostringstream oss;
			oss << m_strProtocol << " " << m_nCode << " " << m_strCodeDesc << "\r\n"
				<< CHttpMessage::GetHeader()
				<< "\r\n" << m_strBody;

			return oss.str();
		}

		void CHttpResMessage::SetCode(int nCode, const std::string &strDesc, const std::string &strProtocol)
		{
			m_nCode = nCode;
			m_strCodeDesc = strDesc;
			m_strProtocol = strProtocol;
		}

		int CHttpResMessage::GetCode() const
		{
			return m_nCode;
		}

		std::string CHttpResMessage::GetCodeDesc() const
		{
			return m_strCodeDesc;
		}

		std::string CHttpResMessage::GetProtocol() const
		{
			return m_strProtocol;
		}
	}
}
