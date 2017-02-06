//
// Created by xobtah on 06/02/17.
//

#ifndef BASIUM_DATABASE_HPP
#define BASIUM_DATABASE_HPP

#include <cstdlib>
#include <ctime>
#include <map>
#include <vector>
#include <exception>

namespace Basium
{
    // DataBaseException
    class DataBaseException : public std::exception
    {
    public:
        DataBaseException(std::string what) throw() : _what(what) {}
        ~DataBaseException() {}

        virtual const char  *what() const throw() { return _what.c_str(); }

    private:
        std::string _what;
    };

    // DataBase
    template<typename T>
    class DataBase
    {
    public:
        DataBase() : _dtorFunc(NULL) {}
        DataBase(void (*dtorFunc)(T&)) : _dtorFunc(dtorFunc) {}
        DataBase(DataBase<T> &db) : _db(db), _dtorFunc(NULL) {}
        DataBase(DataBase<T> &db, void (*dtorFunc)(T&)) : _db(db._db), _dtorFunc(dtorFunc) {}
        ~DataBase()
        {
            if (_dtorFunc)
                for (typename std::map<unsigned int, T>::iterator it = _db.begin(); it != _db.end(); it++)
                    _dtorFunc(it->second);
        }

        // Insert new entry, returns entry's ID in DB
        unsigned int    Insert(T entry)
        {
            unsigned int _id;

            while (_db.find((_id = std::rand())) != _db.end());
            _db[_id] = entry;
            return (_id);
        }

        // Returns a ref to the first entry that returns true when applied to the funcptr function
        T   &FindOne(bool(*funcptr)(const T&))
        {
            for (typename std::map<unsigned int, T>::iterator it = _db.begin(); it != _db.end(); it++)
                if (funcptr(it->second))
                    return (it->second);
            throw DataBaseException("No entry with this query");
        }

        // Returns a ref to the first entry that returns true when applied to the funcptr function with the void* data
        T   &FindOne(bool(*funcptr)(const T&, void*), void *buf)
        {
            for (typename std::map<unsigned int, T>::iterator it = _db.begin(); it != _db.end(); it++)
                if (funcptr(it->second, buf))
                    return (it->second);
            throw DataBaseException("No entry with this query");
        }

        // Returns a ref to the entry which ID is _id
        T   &FindOne(unsigned int _id)
        {
            if (_db.find(_id) == _db.end())
                throw DataBaseException("No entry with this id");
            return (_db[_id]);
        }

        // Returns a vector of pointers to entries that returns true when applied to the funcptr function
        std::vector<T*> Find(bool(*funcptr)(const T&))
        {
            std::vector<T*> subdb;

            for (typename std::map<unsigned int, T>::iterator it = _db.begin(); it != _db.end(); it++)
                if (funcptr(it->second))
                    subdb.push_back(&(it->second));
            return (subdb);
        }

        // Returns a vector of pointers to entries that returns true when applied to the funcptr function with the void* data
        std::vector<T*> Find(bool(*funcptr)(const T&, void*), void *buf)
        {
            std::vector<T*> subdb;

            for (typename std::map<unsigned int, T>::iterator it = _db.begin(); it != _db.end(); it++)
                if (funcptr(it->second, buf))
                    subdb.push_back(&(it->second));
            return (subdb);
        }

        // Removes every entry that returns true when applied to the funcptr function
        int Remove(bool(*funcptr)(const T&))
        {
            int nb = 0;

            for (typename std::map<unsigned int, T>::iterator it = _db.begin(); it != _db.end(); it++)
                if (funcptr(it->second))
                {
                    if (_dtorFunc)
                        _dtorFunc(it->second);
                    _db.remove(it);
                    nb++;
                }
            return (nb);
        }

        // Removes every entry that returns true when applied to the funcptr function with void* data
        int Remove(bool(*funcptr)(const T&, void*), void *buf)
        {
            int nb = 0;

            for (typename std::map<unsigned int, T>::iterator it = _db.begin(); it != _db.end(); it++)
                if (funcptr(it->second, buf))
                {
                    if (_dtorFunc)
                        _dtorFunc(it->second);
                    _db.remove(it);
                    nb++;
                }
            return (nb);
        }

        // Removes the entry of ID _id
        bool    Remove(unsigned int _id)
        {
            if (_db.find(_id) != _db.end())
            {
                if (_dtorFunc)
                    _dtorFunc(_db.find(_id)->second);
                _db.erase(_db.find(_id));
                return (true);
            }
            return (false);
        }
        
        // Get the DB's size
        unsigned int    Size() { return (_db.size()); }

    private:
        std::map<unsigned int, T>   _db;
        void						(*_dtorFunc)(T&);
    };

}

#endif //BASIUM_DATABASE_HPP
