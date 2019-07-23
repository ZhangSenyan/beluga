/**
 * @author Zhang Senyan
 * Date: 2019-04-10
 *
 * @details nocopyable
 * @refitem Shuo Chen (muduo)
 */

#ifndef BELUGA_NOCOPYABLE_H
#define BELUGA_NOCOPYABLE_H

class noncopyable
{
protected:
    noncopyable() {}
    ~noncopyable() {}
private:
    //拷贝构造
    noncopyable(const noncopyable&);

    //拷贝赋值
    const noncopyable& operator=(const noncopyable&);
};

#endif //BELUGA_NOCOPYABLE_H