// -*- mode: c++ -*-

#pragma once

#include <iostream>
#include <string>

/// LogManager - A logger that holds a certain level, and controls the
/// output according to the severity of the message and the current
/// level.
class LogManager {
public:
    /// Enum for severity level.
    enum Level {
        Debug, Info, Warning, Error
    };

    /// LogManager::Logger - Behaves like a stream if the message
    /// level is not lower than the setting.
    class Logger {
    public:
        template <typename T>
        Logger &operator<<(T msg) {
            if (_seversity >= _level) {
                *_output_stream << _level_text[_seversity]
                                << _color_reset << msg << "\n";
            }
            return *this;
        };
        friend class LogManager;
    private:
        Logger(Level sev) : _seversity(sev) { }
        Level _seversity;
        Level _level;
        std::ostream *_output_stream;
    };

    /// Constructor that accept a level. Any message lower than this
    /// level will not be emitted.
    LogManager(Level = Error, std::ostream & = std::cerr);

    /// Change current level settings.
    void setLevel(Level);

    /// Change default output stream.
    void setStream(std::ostream &);

    Logger debug { Debug };
    Logger info { Info };
    Logger warning { Warning };
    Logger error { Error };

private:
    Level _level;
    std::ostream *_output_stream;

    static const std::string _level_text[];
    static const std::string _color_reset;
};

extern LogManager logger;

class ChainedMap {
public:
private:
};
