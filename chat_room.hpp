#pragma once
#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <string>



namespace eosio{
    //using contract::contract;
    //using eosio::name;
    using std::string;


    /// @brief 
    class [[eosio::contract("chat_room")]] room: public contract{
        public:
            
            using contract::contract;

            [[eosio::action]]
            void create(const name& creator, const string& room_name);
            
            [[eosio::action]]
            void select(const name& manager, const name& be_selected, const uint64_t& room_index);
            
            [[eosio::action]]
            void update(const name& manager, const uint64_t& room_index, const string& new_name);
            
            [[eosio::action]]
            void exit(const name& user, const uint64_t& room_index);
            
            [[eosio::action]]
            void send(const name& sender, const uint64_t& room_index, const string& message);
            
            [[eosio::action]]
            void close(const name& manager, const uint64_t& room_index);


            //uint64_t assign(const name& creator, const string& room_name);
            void add_user(const name& manager, const name& be_selected, const uint64_t& room_index);

        private:
            
            struct [[eosio::table]] chat_room{
                name creator;
                string room_name;
                uint64_t room_index;
                uint64_t primary_key() const{
                    return room_index;
                }

            };

            struct [[eosio::table]] user_list{
                name user;

                uint64_t primary_key() const{
                    return user.value;
                }
            };

            struct [[eosio::table]] message_list{
                uint64_t  message_index;
                name sender;
                string message;

                uint64_t primary_key() const{
                    return message_index;
                }
            };

            typedef eosio::multi_index< "chatrooms"_n, chat_room > chat_rooms;
            typedef eosio::multi_index< "userlists"_n, user_list > user_lists;
            typedef eosio::multi_index< "messagelists"_n, message_list > message_lists;

            //name public_code = "rootaccount"_n;
            //name public_code = public_codes;
    };
}
