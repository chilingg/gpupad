/*
 * sigslot优点
 * 不用担心空回调，当回调对象析构时会自动disconnect
 * 支持多线程，线程安全，有锁
 *
 * sigslot缺点
 * 只能回调void类型函数，不支持返回值
 * slot没有优先级，不能动态调整回调队列中的先后顺序
 ×
 * slot函数（被回调的函数）就是普通的成员函数，但有以下限制：
 * 返回值必须为void
 * slot参数个数范围为0-8个
 * 实现slot的类必须继承自has_slots<>
 *
 * Sigslot库用法
 * signal0<> sig1;//没有参数的信号
 * signal2<char *, int> sig2;//发送两个参数
 * sig.sg1.conncent(&slt, &mySlot::on_func1);//连接信号
 * sig.sg1.disconnect(&slt);//断开信号
 * sig.sg1.disconnect_all();//断开全部信号
 * sig.sg1();//发送信号
 * sig.sg1.emit();
 × sig.sg2("str",0.1);
 */
#ifndef RSIGSLOT_H
#define RSIGSLOT_H

#include "Extern/sigslot.h"

class RSigslot: public sigslot::has_slots<>
{
public:
    using Signal0 = sigslot::signal0<>;
    template<typename T1>
    using Signal1 = sigslot::signal1<T1>;
    template<typename T1, typename T2>
    using Signal2 = sigslot::signal2<T1, T2>;
    template<typename T1, typename T2, typename T3>
    using Signal3 = sigslot::signal3<T1, T2, T3>;
    template<typename T1, typename T2, typename T3, typename T4>
    using Signal4 = sigslot::signal4<T1, T2, T3, T4>;
    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    using Signal5 = sigslot::signal5<T1, T2, T3, T4, T5>;
    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    using Signal6 = sigslot::signal6<T1, T2, T3, T4, T5, T6>;
    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    using Signal7 = sigslot::signal7<T1, T2, T3, T4, T5, T6, T7>;
    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    using Signal8 = sigslot::signal8<T1, T2, T3, T4, T5, T6, T7, T8>;

    template<typename Sender, typename Receiver>
    static void connect(const Sender *sender, Signal0 Sender::*signal, const Receiver *receiver, void (Receiver::*slot)())
    {
        (sender->*signal).connect(receiver, slot);
    }

    template<typename Sender, typename Receiver, typename T1>
    static void connect(const Sender *sender, Signal1<T1> Sender::*signal, const Receiver *receiver, void (Receiver::*slot)(T1))
    {
        (sender->*signal).connect(receiver, slot);
    }
};

#endif // RSIGSLOT_H
