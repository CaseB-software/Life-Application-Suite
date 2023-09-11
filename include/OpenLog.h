#ifndef OPEN_LOG
#define OPEN_LOG

/**
 * @file    OpenLog.h
 * @author  Bradley Ryan, Case B Software
 * @brief   Public API header to be included in projects
 * @version 1.0.0-Pre-Release
 * @date    2023-09-10 
 * 
 * @copyright   Copyright (c) 2023
 * 
 */

#include <source_location>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <memory>
#include <chrono>


/**
 * @brief Contains all functionality for interacting with the library
 */
namespace OpenLog {

    /**
     * @brief   Pass these values into changeSettings() to change the configuration of the library 
     * 
     */
    enum SETTINGS {
        SHOW_TIME = 0,
        SHOW_TAGS = 1,
        SHOW_MSG = 2,
        SHOW_LOCATION = 3,
        TAG_TEXT_WIDTH = 4,
        LOG_MSG_MAX_SIZE = 5,
        TIME_OFFSET = 6,
    };
    /**
     * @brief           Input the desired SETTINGS value to then change change the value given. 
     *                  *NOTE* Only TAG_TEXT_WIDTH and LOG_MSG_MAX_SIZE work with the integer as the second parameter function
     * 
     * @param settings   
     * @param value     Using function overrides, you can pass the desired boolean or integer value
     * @return true     The setting was set successfully
     * @return false    The setting could not be changed
     */
    bool    changeSettings(SETTINGS settings, int value);
    /**
     * @brief           Input the desired SETTINGS value to then change change the value given.
     *                  *NOTE* Only TAG_TEXT_WIDTH and LOG_MSG_MAX_SIZE work with the integer as the second parameter function
     * 
     * @param settings   
     * @param value     Using function overrides, you can pass the desired boolean or integer value
     * @return true     The setting was set successfully
     * @return false    The setting could not be changed
     */
    bool    changeSettings(SETTINGS settings, bool value);



     class Tag {
     public:
         Tag(const std::string tag="TAG");
         virtual ~Tag();

        /**
         * @brief Outputs the Tag m_tag name
         * 
         * @return const std::string 
         */
         const std::string str() const noexcept;
         friend std::ostream& operator<<(std::ostream& os, const Tag& tag);

     private:
         std::string m_tag;
     };

    /**
     * @brief       Registers the Tag type within the Open Log application to allow for Tags of that type to be logged.
     * 
     * @param tag   Tag to be registered
     */
     void registerTag       (Tag& tag);
     /**
      * @brief          Check if a Tag with the given string has been registered
      * 
      * @param key      String of the Tag to check
      * @return true    A tag with that key was found
      * @return false   A tag with that key was not found
      */
     bool isTagRegistered   (const std::string key);
     /**
      * @brief          Get the Registered Tag object
      * 
      * @param key      String of the Tag to check
      * @return Tag*    Pointer to the Tag that has been registered
      */
     Tag* getRegisteredTag  (const std::string key);





    class Log{
    public:
        Log(    const std::string msg,
                const std::string tag ="",
                const std::source_location location=std::source_location::current(), 
                const std::chrono::time_point<std::chrono::system_clock> timestamp=std::chrono::system_clock::now()
        );
        virtual ~Log() = default;

        bool addTag(const std::string& tag);

        const std::string 											m_msg;
        const std::source_location									m_location;
        const std::chrono::time_point<std::chrono::system_clock> 	m_timestamp;
              std::vector<std::string>                              m_tags;
    };

    bool log                 (const Log& log);
    bool log                 (const std::ostringstream msg,  const std::string tag = "", const std::source_location location = std::source_location::current());
    bool log                 (const std::string msg,         const std::string tag = "", const std::source_location location = std::source_location::current());

    std::string defaultFormatLog    (const Log& log);

    



    class LogTarget{
    public:
        LogTarget(const std::string name="New Log Target");
        virtual ~LogTarget();
    
        const std::string str() const noexcept;        
        virtual bool LogEvent(const Log& log);

        friend std::ostream& operator<<(std::ostream& os, const LogTarget& lc);

    private:
        const std::string m_name;
    };

    void    registerLogTarget       (std::unique_ptr<LogTarget> target);
    bool    addActiveLogTarget      (const std::string target, const std::source_location location = std::source_location::current());
    bool    removeActiveLogTarget   (const std::string target, const std::source_location location = std::source_location::current());
    
    LogTarget*          getLogTarget(const std::string key);
    std::ostringstream  getAllActiveLogTargets();
}


#endif