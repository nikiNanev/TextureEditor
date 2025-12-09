#include "originator.h"

class Caretaker
{

private:
    std::stack<Memento *> mementos_;
    
    Originator *originator_;

public:
    Caretaker(Originator *originator) : originator_(originator)
    {
    }

    ~Caretaker()
    {
        while (!mementos_.empty())
        {
            mementos_.pop();
        }
        
    }

    void backup()
    {
        std::cout << std::endl;
        std::cout << "Caretaker: Saving Originator's state ..." << std::endl;
        this->mementos_.push(this->originator_->save());
    }

    void undo(loader *loader)
    {
        if (this->mementos_.empty())
            return;

        Memento *memento = this->mementos_.top();
        this->mementos_.pop();

        std::cout << "Caretaker: Restoring state to: " << memento->get_name() << std::endl;
        try
        {
            this->originator_->restore(memento, loader);
        }
        catch (...)
        {
            this->undo(loader);
        }
    }

    void show_history() const
    {
        std::cout << "Caretaker: Here's the list of mementos: " << std::endl;
        
        std::stack<Memento *> history = this->mementos_;
        
        while(!history.empty())
        {
            std::cout << history.top()->get_name() << std::endl;
            history.pop();
        }

    }
};

void memento_cleanup(Originator *originator, Caretaker *caretaker)
{

   delete originator;
   delete caretaker;
}