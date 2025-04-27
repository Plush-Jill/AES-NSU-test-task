//
// Created by plush-jill on 4/24/25.
//

#ifndef ABSTRACT_COMMAND_HPP
#define ABSTRACT_COMMAND_HPP

class AbstractClient;
class AbstractCommand {

public:
    virtual void    execute             (AbstractClient* client) = 0;
    virtual void    execute             (AbstractClient* client, QString* result_message) = 0;
    virtual         ~AbstractCommand    () = default;
};


#endif //ABSTRACT_COMMAND_HPP
