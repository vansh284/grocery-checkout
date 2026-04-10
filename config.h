// config.h
#ifndef CONFIG_H
#define CONFIG_H

#include <QList>
#include <tuple>
#include <QString>

namespace Config {
inline const QList<std::tuple<QString,QString,QString>> LanguageFlags = {
    { ":/img/uk.png",    "en",    "English"  },
    { ":/img/fr.png",    "fr",    "Français" },
    { ":/img/earth.png", "other", "Other"    }
};
}

#endif // CONFIG_H
