//
// Created by Luca Letroye on 28/11/2025.
//

#ifndef PACMAN_SUBJECT_H
#define PACMAN_SUBJECT_H

#include "Observer.h"
#include <vector>
#include <memory>

namespace logic{
    class Subject{
    private:
        std::vector<std::shared_ptr<Observer>> m_observers;
    public:
        virtual ~Subject() = default;

        void attach(std::shared_ptr<Observer> observer) {
            m_observers.push_back(observer);
        }

        void detach(std::shared_ptr<Observer> observer){
            std::erase(m_observers, observer);
        }

    protected:
        void notify(){
            for(std::shared_ptr<Observer> observer : m_observers){
                observer->onNotify(*this);
            }
        }
    };
}

#endif //PACMAN_SUBJECT_H
