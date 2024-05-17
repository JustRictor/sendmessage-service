#ifndef __API_MANAGE_HPP_
#define __API_MANAGE_HPP_

#include <QObject>

class ApiManage : public QObject
{
    Q_OBJECT
public:
    explicit ApiManage(QObject *parent = nullptr);

signals:
};

#endif // API_MANAGE_HPP_
