#include "Infrastructure.h"
#include "Modules/Textures/Loader.h"

class Originator
{
private:
    std::string state_;
    std::stack<SDL_Texture *> textures;
    std::string generate_random_string(int length = 10)
    {
        const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
        int stringLength = sizeof(alphanum) - 1;

        std::string random_string;
        for (int i = 0; i < length; i++)
        {
            random_string += alphanum[std::rand() % stringLength];
        }
        return random_string;
    }

public:
    Originator(std::string state) : state_(state)
    {
        std::cout << "Originator: My initial state is: " << this->state_ << std::endl;
    }

    void save_snapshot(SDL_Texture *texture)
    {
        std::cout << "Originator: snapshot being saved" << std::endl;
        this->state_ = this->generate_random_string(30);
        this->textures.push(texture);
        std::cout << "Originator: and my state has changed to: " << this->state_ << std::endl;
    }

    void save_action(const std::string action)
    {
        std::cout << "Originator: action being saved" << std::endl;
        this->state_ = this->generate_random_string(30);
        std::cout << "Action: " << action << "\t state: " << this->state_ << std::endl;
    }

    Memento *save()
    {
        return new Infrastructure(this->state_);
    }

    void restore(Memento *memento, Loader *loader)
    {
        this->state_ = memento->state();
        
        std::cout << "Originator: My state has changed to: " << this->state_ << std::endl;
        if(!this->textures.empty())
        {
            loader->set_texture(this->textures.top());
            this->textures.pop();
        }
            
        delete memento;
    }
};