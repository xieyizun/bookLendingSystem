#include <stdio.h>
#include <iostream>
#include <cstring>
#include <sstream>
#include <map>
#include "SessionController.h"

SessionController::SessionController() {
	userManager = UserManager::getInstance();
	bookManager = BookManager::getInstance();
	recordManager = RecordManager::getInstance();
}

SessionController::~SessionController() {
	printf("\n 谢谢,欢迎再次登陆图书借阅系统!\n\n");
}

std::vector<User> SessionController::getAllUsersFromMySQL() {
	std::vector<User> result;

	const char *sql = "SELECT userID, userName, userPassword, userEmail, userPhone,userAddress, money FROM user";

	int userID;

	unsigned long userName_len;
	char userName[25];

	unsigned long userPassword_len;
	char userPassword[25];

	unsigned long userEmail_len;
	char userEmail[25];

	unsigned long userPhone_len;
	char userPhone[25];

	unsigned long userAddress_len;
	char userAddress[100];

	double userMoney;

	mysql_stmt_prepare(stmt, sql, strlen(sql));
	memset(userB, 0, sizeof(userB));
	userB[0].buffer_type = FIELD_TYPE_LONG;
	userB[0].buffer = &userID;

	userB[1].buffer_type = FIELD_TYPE_VAR_STRING;
	userB[1].buffer = userName;
	userB[1].buffer_length = 25;
	userB[1].length = &userName_len;

	userB[2].buffer_type = FIELD_TYPE_VAR_STRING;
	userB[2].buffer = userPassword;
	userB[2].buffer_length = 25;
	userB[2].length = &userPassword_len;

	userB[3].buffer_type = FIELD_TYPE_VAR_STRING;
	userB[3].buffer = userEmail;
	userB[3].buffer_length = 25;
	userB[3].length = &userEmail_len;

	userB[4].buffer_type = FIELD_TYPE_VAR_STRING;
	userB[4].buffer = userPhone;
	userB[4].buffer_length = 25;
	userB[4].length = &userPhone_len;

	userB[5].buffer_type = FIELD_TYPE_VAR_STRING;
	userB[5].buffer = userAddress;
	userB[5].buffer_length = 100;
	userB[5].length = &userAddress_len;

	userB[6].buffer_type = FIELD_TYPE_DOUBLE;
	userB[6].buffer = &userMoney;

	mysql_stmt_bind_result(stmt, userB);//bind result

	if (mysql_stmt_execute(stmt)) { //execute
		fprintf(stderr, "sorry, an error happened when executing stmt ...\n");
		return result;
	}

	mysql_stmt_store_result(stmt); // get result from mysql server to memory

	//loop through all result records
	while (!mysql_stmt_fetch(stmt)) {
		std::string name(userName, userName+strlen(userName));
		std::string password(userPassword, userPassword+strlen(userPassword));
		std::string email(userEmail, userEmail+strlen(userEmail));
		std::string phone(userPhone, userPhone+strlen(userPhone));
		std::string address(userAddress, userAddress+strlen(userAddress));
		double money = userMoney;

		User temp(userID, name, password, email, phone, address, money);
		result.push_back(temp);
	}
	return result;
}

std::vector<Book> SessionController::getAllBooksFromMySQL() {
	std::vector<Book> result;
	const char *sql = "SELECT bookID, bookName, ownerName, ownerID, price, state FROM book";
	int bookID;

	unsigned long bookName_len;
	char bookName[55];

	unsigned long ownerName_len;
	char ownerName[25];

	int ownerID;

	int state;

	double bookPrice;

	mysql_stmt_prepare(stmt, sql, strlen(sql));
	memset(bookB, 0, sizeof(bookB));
	bookB[0].buffer_type = FIELD_TYPE_LONG;
	bookB[0].buffer = &bookID;

	bookB[1].buffer_type = FIELD_TYPE_VAR_STRING;
	bookB[1].buffer = bookName;
	bookB[1].buffer_length = 55;
	bookB[1].length = &bookName_len;

	bookB[2].buffer_type = FIELD_TYPE_VAR_STRING;
	bookB[2].buffer = ownerName;
	bookB[2].buffer_length = 25;
	bookB[2].length = &ownerName_len;

	bookB[3].buffer_type = FIELD_TYPE_LONG;
	bookB[3].buffer = &ownerID;

	bookB[4].buffer_type = FIELD_TYPE_DOUBLE;
	bookB[4].buffer = &bookPrice;

	bookB[5].buffer_type = FIELD_TYPE_LONG;
	bookB[5].buffer = &state;


	mysql_stmt_bind_result(stmt, bookB);//bind result

	if (mysql_stmt_execute(stmt)) { //execute
		fprintf(stderr, "sorry, an error happened when executing stmt ...\n");
		return result;
	}

	mysql_stmt_store_result(stmt); // get result from mysql server to memory

	//loop through all result records
	while (!mysql_stmt_fetch(stmt)) {
		std::string bname(bookName, bookName+strlen(bookName));
		std::string oname(ownerName, ownerName+strlen(ownerName));
		Book temp(bookID, bname, oname, ownerID, bookPrice, state);
		result.push_back(temp);
	}
	return result;
}

std::vector<Record> SessionController::getAllRecordFromMySQL() {
	std::vector<Record> result;

	const char *sql = "SELECT recordID, bookName, bookID, lenderID, borrowerID, price, \
					   borrowTime, completed FROM record";

	int recordID;

	unsigned long bookName_len;
	char bookName[55];
	int bookID;
	int lenderID;
	int borrowerID;
	int completed;
	int borrowTime;
	double price;

	mysql_stmt_prepare(stmt, sql, strlen(sql));

	memset(recordB, 0, sizeof(recordB));

	recordB[0].buffer_type = FIELD_TYPE_LONG;
	recordB[0].buffer = &recordID;

	recordB[1].buffer_type = FIELD_TYPE_VAR_STRING;
	recordB[1].buffer = bookName;
	recordB[1].buffer_length = 55;
	recordB[1].length = &bookName_len;

	recordB[2].buffer_type = FIELD_TYPE_LONG;
	recordB[2].buffer = &bookID;

	recordB[3].buffer_type = FIELD_TYPE_LONG;
	recordB[3].buffer = &lenderID;

	recordB[4].buffer_type = FIELD_TYPE_LONG;
	recordB[4].buffer = &borrowerID;

	recordB[5].buffer_type = FIELD_TYPE_DOUBLE;
	recordB[5].buffer = &price;

	recordB[6].buffer_type = FIELD_TYPE_LONG;
	recordB[6].buffer = &borrowTime;

	recordB[7].buffer_type = FIELD_TYPE_LONG;
	recordB[7].buffer = &completed;

	mysql_stmt_bind_result(stmt, recordB);//bind result

	if (mysql_stmt_execute(stmt)) { //execute
		fprintf(stderr, "sorry, an error happened when executing stmt ...\n");
		return result;
	}

	mysql_stmt_store_result(stmt); // get result from mysql server to memory

	//loop through all result records
	while (!mysql_stmt_fetch(stmt)) {
		std::string bname(bookName, bookName+strlen(bookName));
		Record temp(recordID, bname, bookID, lenderID, borrowerID, price, borrowTime, completed);
		result.push_back(temp);
	}
	return result;
}

bool SessionController::userExist(const std::string &userName) {
	User *exist = userManager->findUserByName(userName);
	if (exist == NULL) {
		return false;
	}
	return true;
}

void SessionController::startBookSystem() {
	conn = mysql_init(NULL);
	mysql_options(conn, MYSQL_READ_DEFAULT_GROUP, "");
	//connection
	if (mysql_real_connect(conn, "localhost", "root", "jkl",
							"book_system", 0, NULL, 0) == NULL) {
		fprintf(stderr, "sorry, no database connection\n");
		return;
	}

	mysql_set_server_option(conn, MYSQL_OPTION_MULTI_STATEMENTS_ON);
	mysql_query(conn, "SET NAMES 'utf-8'");
	//pre statements
	stmt = mysql_stmt_init(conn);
	//read users, books and records from MySQL
	userManager->initUsers(getAllUsersFromMySQL());

	std::vector<Book> tempBooks = getAllBooksFromMySQL();
	std::multimap<std::string, Book> books;
	std::multimap<int, Book> books2;
	for (std::vector<Book>::iterator iter = tempBooks.begin(); iter != tempBooks.end(); iter++) {
		books.insert(make_pair(iter->getName(), *iter));
		books2.insert(std::make_pair(iter->getOwnerID(), *iter));
	}
	bookManager->initBooks(books);
	bookManager->initBooks2(books2);
	
	recordManager->initRecords(getAllRecordFromMySQL());

	mysql_stmt_close(stmt); //获得所有查询结果记录后,释放MYSQL_STMT结构占用的内存
}

void SessionController::quitBookSystem() {
	mysql_close(conn);
}

User* SessionController::userLogIn(const std::string &userName, const std::string &password) {
	User *user = userManager->findUserByName(userName);
	if (user != NULL && password.compare(user->getPassword()) == 0) {
		return user;
	} else {
		return NULL;
	}
}

bool SessionController::userRegister(std::string userName, std::string userPassword, std::string userEmail,
						  std::string userPhone, std::string userAddress, double money) {
	std::string insert = "INSERT INTO user (userName, userPassword, userEmail,\
						 userPhone, userAddress, money) VALUES ('";
	insert += userName;
	insert += "', '";
	insert += userPassword;
	insert += "', '";
	insert += userEmail;
	insert += "', '";
	insert += userPhone;
	insert += "', '";
	insert += userAddress;
	insert += "', '";

	std::ostringstream os;
	os << money;
	insert += os.str();
	insert += "')";

	if (mysql_query(conn, insert.c_str())) {
		fprintf(stderr, "MySQL error: %s\n", mysql_error(conn));
		return false;
	}
	int userID = (int)mysql_insert_id(conn);
	userManager->createUser(userID, userName, userPassword, userEmail, userPhone, userAddress, money);
	return true;
}

bool SessionController::modifyUserInfo(User &user, int select, const std::string &content) {
	std::string update = "UPDATE user SET ";
	std::string who = "' WHERE userID=";
	std::ostringstream os1;
	os1 << user.getUserID();
	who += os1.str();

	bool success = false;

	switch(select) {
		case 1: { //名字
			update += "userName='";
			update += content;
			update += who;
			break;
		}
		case 2: { //密码
			update += "userPassword='";
			update += content;
			update += who;
			break;
		}
		case 3: { //邮箱
			update += "userEmail='";
			update += content;
			update += who;
			break;
		}
		case 4: { //电话号码
			update += "userPhone='";
			update += content;
			update += who;
			break;
		}
		case 5: { //地址
			update += "userAddress='";
			update += content;
			update += who;
			break;
		}
	}

	if (mysql_query(conn, update.c_str())) {
		fprintf(stderr, "MySQL Error: %s\n", mysql_error(conn));
		return false;
	}

	switch(select) {
		case 1: {
			userManager->updateUserName(user, content);
			break;
		}
		case 2: {
			userManager->updateUserPassword(user, content);
			break;
		}
		case 3: {
			userManager->updateUserEmail(user, content);
			break;
		}
		case 4: {
			userManager->updateUserPhone(user, content);
			break;
		}
		case 5: {
			userManager->updateUserAddress(user, content);
			break;
		}
	}
	return true;
}

bool SessionController::addUserMoney(User &user, double money) {
	std::string update = "UPDATE user SET money=money+";
	std::ostringstream os1;
	os1 << money;
	update += os1.str();
	update += " WHERE userID=";
	std::ostringstream os2;
	os2 << user.getUserID();
	update += os2.str();

	if (mysql_query(conn, update.c_str())) {
		fprintf(stderr, "MySQL Error: %s\n", mysql_error(conn));
		return false;
	}
	userManager->updateUserMoney(user, user.getMoney()+money);
	return true;
}

std::vector<User> SessionController::listAllUsers() {
	return userManager->listAllUsers();
}

bool SessionController::addBook(std::string bookName, User &owner, double price) {
	std::string insert = "INSERT INTO book (bookName, ownerName, ownerID, price) VALUES ('";
	insert += bookName;
	insert += "', '";
	insert += owner.getName();
	insert += "', ";

	std::ostringstream os1;
	os1 << owner.getUserID();
	insert += os1.str();
	insert += ", ";

	std::ostringstream os2;
	os2 << price;
	insert += os2.str();
	insert += ")";
	
	if (mysql_query(conn, insert.c_str())) {
		fprintf(stderr, "Mysql Error: %s\n", mysql_error(conn));
		return false;
	}

	int bookID = (int)mysql_insert_id(conn);

	bookManager->addBook(bookID, bookName, owner.getName(), owner.getUserID(), price);

	return true;
}

std::vector<Book*> SessionController::queryBook(std::string bookName) {
	return bookManager->searchBook(bookName);
}

bool SessionController::lendBook(User &borrower, Book &book, int borrowTime) {
	if (borrower.getMoney() < book.getPrice()) {
		fprintf(stderr, "您的账户余额不足,请及时充值!\n");
		return false;
	}

	//create a lend book record
	std::string insert = "INSERT INTO record (bookName, bookID, lenderID, \
						  borrowerID, price, borrowTime) VALUES ('";
	insert += book.getName();
	insert += "', ";

	std::ostringstream os1;
	os1 << book.getBookID();
	insert += os1.str();
	insert += ", ";

	std::ostringstream os2;
	os2 << book.getOwnerID();
	insert += os2.str();
	insert += ", ";

	std::ostringstream os3;
	os3 << borrower.getUserID();
	insert += os3.str();
	insert += ", ";
	
	std::ostringstream os4;
	os4 << book.getPrice();
	insert += os4.str();
	insert += ", ";

	std::ostringstream os5;
	os5 << borrowTime;
	insert += os5.str();
	insert += ");";
	
	//update the lender's money in account
	double money = borrower.getMoney()-book.getPrice();

	std::string updateMoney = "UPDATE user SET money=";
	std::ostringstream os7;
	os7 << money;
	updateMoney += os7.str();

	std::ostringstream os8;
	updateMoney += " WHERE userID=";
	os8 << borrower.getUserID();
	updateMoney += os8.str();
	updateMoney += ";";

	std::string updateBook = "UPDATE book SET state=0 WHERE bookID=";
	std::ostringstream os9;
	os9 << book.getBookID();
	updateBook += os9.str();

	//execute multi statements at one time
	std::string multiOp(insert+updateMoney+updateBook);

	if (mysql_query(conn, multiOp.c_str())) {
		fprintf(stderr, "MySQL Error: %s\n", mysql_error(conn));
		return false;
	}
	int recordID = (int)mysql_insert_id(conn);
	//mysql_query同时执行多条sql命令之后需要清空内存中从服务器读来的数据,才能在该同一个连接中连续执行多条sql语句,否则会报错:
	//MySQL Error: 1024 Commands out of sync; you can't run this command now
	MYSQL_RES *result = mysql_store_result(conn);
	MYSQL_ROW row;
	int next;
	do { //loop over all results
		result = mysql_store_result(conn);

		if (result) {
			while ((row = mysql_fetch_row(result)) != NULL);
			mysql_free_result(result);
		}
		//read next result
		next = mysql_next_result(conn);
	} while (!next);

	recordManager->addRecord(Record(recordID, book.getName(), book.getBookID(), book.getOwnerID(), 
							        borrower.getUserID(), book.getPrice(), borrowTime, false));

	userManager->updateUserMoney(borrower, money);

	bookManager->updateBookState(book);

	return true;
}

bool SessionController::returnBook(User &borrower, Record &record) {
	std::string updateRecord = "UPDATE record SET completed=1 WHERE recordID=";
	std::ostringstream os1;
	os1 << record.getRecordID();
	updateRecord += os1.str(); 
	updateRecord += ";";

	std::string updateUser = "UPDATE user SET money=money+";
	std::ostringstream os2;
	os2 << record.getPrice();
	updateUser += os2.str();
	updateUser += " WHERE userID=";

	std::ostringstream os3;
	os3 << borrower.getUserID();
	updateUser += os3.str();
	updateUser += ";";

	std::string updateBook = "UPDATE book SET state=1 WHERE bookID=";
	
	std::ostringstream os4;
	os4 << record.getBookID();
	updateBook += os4.str();

	std::string update(updateRecord+updateUser+updateBook);

	if (mysql_query(conn, update.c_str())) {
		fprintf(stderr, "MySQL Error: %s\n", mysql_error(conn));
		return false;
	}
	//mysql_query同时执行多条sql命令之后,需要清空内存中从服务器读来的数据,才能在该同一个连接中连续执行多条sql语句,否则会报错:
	//MySQL Error: 1024 Commands out of sync; you can't run this command now
	MYSQL_RES *result = mysql_store_result(conn);
	MYSQL_ROW row;
	int next;
	do { //loop over all results
		result = mysql_store_result(conn);

		if (result) {
			while ((row = mysql_fetch_row(result)) != NULL);
			mysql_free_result(result);
		}
		//read next result
		next = mysql_next_result(conn);
	} while (!next);

	record.setCompleted();

	userManager->updateUserMoney(borrower, borrower.getMoney()+record.getPrice());

	std::vector<Book*> books(queryBook(record.getBookName()));

	Book *book = NULL;
	for (std::vector<Book*>::iterator iter = books.begin(); iter != books.end(); iter++) {
		if ((*iter)->getBookID() == record.getBookID()) {
			book = *iter;
			break;
		}
	}
	if (book != NULL)
		bookManager->updateBookState(*book);

	return true;
}

std::vector<Book*> SessionController::listAllMyBooks(int userID) {
	return bookManager->searchBookByUserID(userID);
}

std::vector<Record*> SessionController::listAllMyRecords(int userID) {
	return recordManager->listAllMyRecords(userID);
}

std::vector<Record*> SessionController::listMyBorrowerRecords(int userID) {
	return recordManager->listMyBorrowerRecords(userID);
}

std::vector<Record*> SessionController::listMyLenderRecords(int userID) {
	return recordManager->listMyLenderRecords(userID);
}

bool SessionController::deleteBook(const User &owner, const Book &book) {
	std::vector<Record*> myLenderRecords = listMyLenderRecords(owner.getUserID());
	for (std::vector<Record*>::iterator iter = myLenderRecords.begin(); iter != myLenderRecords.end(); iter++) {
		if ((*iter)->getBookID() == book.getBookID() && (*iter)->getCompleted() == false) {
			return false;
		}
	}
	std::string deleteBook = "DELETE FROM book WHERE bookID=";
	std::ostringstream os;
	os << book.getBookID();
	deleteBook += os.str();
	deleteBook += ";";

	if (mysql_query(conn, deleteBook.c_str())) {
		fprintf(stderr, "MySQL Error: %s\n", mysql_error(conn));
		return false;
	}

	bookManager->deleteBook(owner.getName(), book.getName());
	return true;
}

bool SessionController::deleteRecord(const User &user, const Record &record) {
	if (record.getCompleted() == false) {
		return false;
	}
	std::string deleteRecord = "DELETE FROM record WHERE recordID=";
	std::ostringstream os;
	os << record.getRecordID();
	deleteRecord += os.str();

	if (mysql_query(conn, deleteRecord.c_str())) {
		fprintf(stderr, "MySQL Error: %s\n", mysql_error(conn));
		return false;
	}
	recordManager->deleteRecord(record);
	return true;
}

bool SessionController::deleteUser(const User& user) {
	std::vector<Record*> allMyRecords = listAllMyRecords(user.getUserID());
	for (std::vector<Record*>::iterator iter = allMyRecords.begin(); iter != allMyRecords.end(); iter++) {
		if ((*iter)->getCompleted() == false) {
			if ((*iter)->getLenderID() == user.getUserID()) {
				std::cout << " 您当前还有借出去的书未被还回来!" << std::endl;
			} else {
				std::cout << " 您当前还有借的书未还回去的!" << std::endl;
			}
			return false;
		}
	}
	std::string deleteUser = "DELETE FROM user WHERE userID=";
	std::ostringstream os;
	os << user.getUserID();
	deleteUser += os.str();

	if (mysql_query(conn, deleteUser.c_str())) {
		fprintf(stderr, "MySQL Error: %s\n", mysql_error(conn));
		return false;
	}

	return userManager->deleteUser(user);
}