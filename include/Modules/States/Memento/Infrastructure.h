#include "Memento.h"

class Infrastructure : public Memento
{
private:
    std::string state_;
    std::string date_;

public:
    Infrastructure(std::string state) : state_(state)
    {
        std::time_t now = std::time(0);
        this->date_ = std::ctime(&now);
        
    }

    std::string state() const override
    {
        std::cout << "Infrastructure override of state method" << std::endl;
        return this->state_;
    }

    std::string get_name() const override
    {
        return this->date_ + " / ( " + this->state_.substr(0, 9) + " ...)";
    }

    std::string date() const override
    {
        return this->date_;
    }
};
