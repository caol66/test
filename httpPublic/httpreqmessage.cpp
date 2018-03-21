#include "httpreqmessage.h"
#include <sstream>
#include <vector>

#include "unmutility/util/StringUtil.h"

namespace unm
{
	namespace http
	{

		CHttpReqMessage::CHttpReqMessage()
		{
		}

		CHttpReqMessage::~CHttpReqMessage()
		{
		}

		bool CHttpReqMessage::SetMessage(const std::string &strHeader, const std::string &strBody)
		{
			bool bSuccess = false;

			do
			{
				m_strMethod.clear();
				m_strUri.clear();
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

				std::vector<std::string> vecReqLine;
				unm::StringUtil::Split(vecReqLine,strReqLine," ");
				if (vecReqLine.size() != 3)
				{
					break;
				}

				m_strMethod = vecReqLine[0];
				SetUrl(vecReqLine[1]);
				m_strProtocol = vecReqLine[2];

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

		std::string CHttpReqMessage::GetHeader() const
		{
			std::ostringstream oss;
			oss << m_strMethod << " " << m_strUrl << " " << m_strProtocol << "\r\n"
				<< CHttpMessage::GetHeader();

			return oss.str();
		}

		std::string CHttpReqMessage::GetPacket() const
		{
			std::ostringstream oss;
			oss << m_strMethod << " " << m_strUrl << " " << m_strProtocol << "\r\n"
				<< CHttpMessage::GetHeader()
				<< "\r\n" << m_strBody;

			return oss.str();
		}

		void CHttpReqMessage::SetRequestLine(const std::string &strMethod, const std::string &strUrl, const std::string &strProtocol)
		{
			SetUrl(strUrl);
			m_strMethod = strMethod;
			m_strProtocol = strProtocol;
		}

		std::string CHttpReqMessage::GetMethod() const
		{
			return m_strMethod;
		}

		std::string CHttpReqMessage::GetUri() const
		{
			return m_strUri;
		}

		std::string CHttpReqMessage::GetProtocol() const
		{
			return m_strProtocol;
		}

		bool CHttpReqMessage::DesGetParameterExist(const std::string &strParamter) const
		{
			return (m_mapParameters.find(strParamter) != m_mapParameters.end());
		}

		std::string CHttpReqMessage::GetParameter(const std::string &strParamter) const
		{
			std::map<std::string, std::string>::const_iterator it = m_mapParameters.find(strParamter);
			if (it != m_mapParameters.end())
			{
				return it->second;
			}

			return "";
		}

		void CHttpReqMessage::SetParameter(const std::string &strParamter, const std::string &strValue)
		{
			m_mapParameters[strParamter] = strValue;
			ComposeUrl();
		}

		void CHttpReqMessage::RemoveParameter(const std::string &strParamter)
		{
			m_mapParameters.erase(strParamter);
			ComposeUrl();
		}

		void CHttpReqMessage::SetUrl(const std::string &strUrl)
		{
			do
			{
				m_strUri.clear();
				m_mapParameters.clear();

				// 保存完整的 Url 地址
				m_strUrl = strUrl;

				// 解析出 Uri 和 Parameters
				// 查找第一个 ?, 后续的字符串为 key1=value1&key2=value2...
				std::string::size_type nPos = strUrl.find('?');
				if (nPos == std::string::npos)
				{
					m_strUri = strUrl;
					break;
				}

				m_strUri.assign(strUrl.c_str(), nPos);
				++nPos;

				std::vector<std::string> vecLine;
				unm::StringUtil::Split(vecLine,strUrl,"&");
				std::vector<std::string>::const_iterator it = vecLine.begin();
				for (; it != vecLine.end(); ++it)
				{
					// 查找 '=' 符，取出 key/value 对
					std::string::size_type nSeprator = it->find('=');
					if (nSeprator == std::string::npos)
					{
						continue;
					}

					std::string strKey(it->c_str(), nSeprator);
					m_mapParameters[strKey] = it->c_str() + nSeprator + 1;
				}
			}
			while (0);
		}

		void CHttpReqMessage::ComposeUrl()
		{
			std::ostringstream oss;
			for (std::map<std::string, std::string>::const_iterator it = m_mapParameters.begin(); it != m_mapParameters.end(); ++it)
			{
				oss << "&" << it->first << "=" << it->second;
			}

			m_strUrl = m_strUri + "?" + (oss.str().c_str() + 1);
		}
	}
}
