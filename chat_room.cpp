#include </Users/d3j/Documents/chat_room/chat_room.hpp>

namespace eosio {

    void room::create(const name& creator, const string& room_name){
        
        require_auth(creator);
        //uint64_t index = assign(creator, room_name);
        chat_rooms chat_roomstable(get_self(), get_self().value);
        //uint64_t index = 0;
        //auto existing = chat_roomstable.find(room_name);
        //check( existing == chat_roomstable.end(), 'The chat_room of this creator has been create');

        auto index = chat_roomstable.available_primary_key();

        chat_roomstable.emplace(creator, [&](auto& s){
                s.creator = creator;
                s.room_name = room_name;
                s.room_index = index;
        });

        add_user(creator, creator, index);
    }

    void room::select(const name& manager, const name& be_selected, const uint64_t& room_index){
        require_auth(manager);
        chat_rooms chat_roomstable(get_self(), get_self().value);
        auto existing = chat_roomstable.find(room_index);
        check( existing != chat_roomstable.end(), "The chat_room of this creator has not been create");
        add_user(manager, be_selected, room_index);
    }

    void room::update(const name& manager, const uint64_t& room_index, const string& new_name){
        
        require_auth(manager);
        chat_rooms chat_roomstable(get_self(), get_self().value);
        auto st = chat_roomstable.find(room_index);
        check( st != chat_roomstable.end(), "The chat_room of this creator has not been created");

        chat_roomstable.modify(st, manager, [&](auto& s){
            s.room_name = new_name;
        } );
    }

    void room::exit(const name& user, const uint64_t& room_index){
        //check(creator != name, 'creator of chat_room can not exit the room, but can only delete the room.')
        //require_auto(creator);
        require_auth(user);

        chat_rooms chat_roomstable(get_self(), get_self().value);
        auto ex = chat_roomstable.find(room_index);
        //check( st != chat_roomstable.end(), "The chat_room of this creator has not been created");



        user_lists user_liststable(get_self(), room_index);
        auto st = user_liststable.find(user.value);
        check( st != user_liststable.end(), "the user dose not exist in the room");
        check( ex->creator != user, "the creator can not exit from room, but can only delete it");

        user_liststable.erase(st);
    }


    void room::send(const name& sender, const uint64_t& room_index, const string& message){
        require_auth(sender);
        user_lists user_liststable(get_self(), room_index);
        auto st = user_liststable.find(sender.value);
        check( st != user_liststable.end(), "the user dose not exist in the room");


        message_lists message_liststable(get_self(), room_index);
        auto id = message_liststable.available_primary_key();
        message_liststable.emplace(sender, [&](auto& s){
            s.message = message;
            s.message_index = message_liststable.available_primary_key();
            s.sender = sender;
        });

    }


    void room::close(const name& manager, const uint64_t& room_index)
    {
        require_auth(manager);
        chat_rooms chat_roomstable(get_self(), manager.value);
        auto st = chat_roomstable.find(room_index);
        check(st != chat_roomstable.end(), "room do not exist");

        chat_roomstable.erase(st);

        user_lists user_liststable(get_self(), room_index);
        for (auto it = user_liststable.begin(); it != user_liststable.end();)
            user_liststable.erase(it++);

        message_lists message_liststable(get_self(), room_index);
        for (auto it = message_liststable.begin(); it != message_liststable.end();)
            message_liststable.erase(it++);
    }

    void room::add_user(const name& manager, const name& be_selected, const uint64_t& room_index){
        
        user_lists user_liststable(get_self(), room_index);

        //user_lists user_liststable(get_self(), room_index);
        auto exi2 = user_liststable.find(be_selected.value);
        check(exi2 == user_liststable.end(), "User was added in this room already"); 

        user_liststable.emplace(manager, [&](auto& s){
            s.user = be_selected;
        });
    }
}