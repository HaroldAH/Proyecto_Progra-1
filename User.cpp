#include "User.h"


void User::expandAndAssignUsers(User &usersObj, int quantity) {
    
    if (usersObj.userCount + quantity > usersObj.capacity) {
        
        int newCap = (usersObj.capacity == 0) ? quantity : usersObj.capacity + quantity;

        UserData* newArr = new UserData[newCap];
        
        for (int i = 0; i < usersObj.userCount; i++) {
            newArr[i] = usersObj.users[i];
        }
        
        delete[] usersObj.users;
        usersObj.users = newArr;
        usersObj.capacity = newCap;
    }
}


void User::createUser(User &usersObj) {
    int quantity = 0;
    std::cout << "¿Cuántos usuarios desea agregar? ";
    quantity = getValidIntInput();   


    expandAndAssignUsers(usersObj, quantity);

    for (int i = 0; i < quantity; i++) {
        std::string idNumber, name, birthDate;

        std::cout << "\nIngrese la cédula del usuario " 
                  << usersObj.userCount + 1 << ": ";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, idNumber);

        std::cout << "Ingrese el nombre del usuario " 
                  << usersObj.userCount + 1 << ": ";
        std::getline(std::cin, name);

        std::cout << "Ingrese la fecha de nacimiento del usuario " 
                  << usersObj.userCount + 1 << " (DD/MM/AAAA): ";
        std::getline(std::cin, birthDate);

        usersObj.users[usersObj.userCount].setIdNumber(idNumber);
        usersObj.users[usersObj.userCount].setName(name);
        usersObj.users[usersObj.userCount].setBirthDate(birthDate);

        usersObj.userCount++;
        std::cout << "Usuario registrado exitosamente.\n";
    }

    std::cout << "\nSe han registrado " << quantity << " usuarios correctamente.\n";
}


void User::createUser(User &usersObj, const std::string &idNumber) {
    
    expandAndAssignUsers(usersObj, 1);

    
    usersObj.users[usersObj.userCount].setIdNumber(idNumber);

    
    std::string name, birthDate;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Ingrese el nombre del usuario " << usersObj.userCount + 1 << ": ";
    std::getline(std::cin, name);
    usersObj.users[usersObj.userCount].setName(name);

    std::cout << "Ingrese la fecha de nacimiento del usuario " 
              << usersObj.userCount + 1 << " (DD/MM/AAAA): ";
    std::getline(std::cin, birthDate);
    usersObj.users[usersObj.userCount].setBirthDate(birthDate);

    
    usersObj.userCount++;

    std::cout << "Usuario registrado exitosamente con la cédula " 
              << idNumber << ".\n";
}


UserData* User::searchUserById(std::string &idNumber) {
    for (int i = 0; i < userCount; i++) {
        if (users[i].getIdNumber() == idNumber) {
            return &users[i];
        }
    }
    return nullptr;
}
