#ifndef SINGLETON_HPP
#define SINGLETON_HPP
#include<memory>
#include<mutex>


template<typename SType>
class TSingleton
{
public:
    template<typename... Args>
    static std::shared_ptr<SType> GetInstance(Args&&... args)
    {
        std::lock_guard<std::mutex> lg(mutexLock);
        if (pInstance == nullptr)
        {
            // ȱ�� ���ò���˽�еĹ��캯�� ���Ƕ�Ӧ�ཫstd::make_shared����Ϊ��Ԫ����
            // pInstance = std::make_shared<SType>(std::forward<Args>(args)...);
            pInstance = std::shared_ptr<SType>(new SType(std::forward<Args>(args)...));
        }
        return pInstance;
    }

    TSingleton() = delete;
    TSingleton(const TSingleton&) = delete;
    TSingleton& operator=(const TSingleton&) = delete;
private:
    static std::shared_ptr<SType> pInstance;
    static std::mutex mutexLock;
};

template<typename SType>
std::shared_ptr<SType> TSingleton<SType>::pInstance = nullptr;

template<typename SType>
std::mutex TSingleton<SType>::mutexLock;

#endif