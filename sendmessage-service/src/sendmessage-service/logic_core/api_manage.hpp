#ifndef __API_MANAGE_HPP_
#define __API_MANAGE_HPP_

#include <QObject>
#include <QHash>
#include <QVector>
#include <QString>

class ApiManage : public QObject
{
    Q_OBJECT
private:
    QHash<QString, QVector<QString>> tokens{
        { "17a65071c496aff94e9ae6a296724beb", {"all"} },
    };
public:
    explicit ApiManage(QObject *parent = nullptr);
    bool isValid(QString const &token, QString const &find_method);
signals:
};

#endif // API_MANAGE_HPP_
