#ifndef DEBUG_HPP
#define DEBUG_HPP


#include <fstream>
#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "system.hpp"

class Debug
{
public:
	Debug(const std::string &file, const std::string &func, int line, ...)
		: m_hasEntries(false)
	{
		if(!m_stream.is_open()) {
			m_stream.open(RpgSaveLoadImpl::System::GetCurrentPath()
				+ RpgSaveLoadImpl::System::DirectorySeparatorChar() + "RpgSaveLoadDebug.log",
				std::ios_base::out | std::ios_base::app);
			m_stream.imbue(std::locale(m_stream.getloc(), new boost::posix_time::time_facet()));
		}

		m_stream << '[' << boost::posix_time::second_clock::local_time() << " " << line << " " << func << " " << file << "] " << std::endl;
	}

    ~Debug()
	{
		m_stream << std::endl << std::endl << std::endl;
        m_stream.flush();
        m_stream.close();
	}

    template<typename ObjectType>
    Debug &operator , (const ObjectType &v)
	{
		if (m_hasEntries)
			m_stream << std::endl;
		m_stream << v;
		m_hasEntries = true;
		return *this;
	}

private:
    std::ofstream m_stream;
    bool m_hasEntries;
};

#define LOG(...)	Debug(__FILE__, __FUNCTION__, __LINE__), __VA_ARGS__;


#endif /* DEBUG_HPP */

