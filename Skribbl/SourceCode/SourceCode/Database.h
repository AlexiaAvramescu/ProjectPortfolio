#pragma once
#include <string>
#include <sqlite_orm/sqlite_orm.h>
#include <optional>
#include <vector>
#include <fstream>

#include <chrono>
#include <format>
#include <sstream>
#include <memory>

static std::string sysDaysToString(std::chrono::sys_days pt) {
	auto r = std::format("{:%F}", pt);
	return r;
}

static std::optional<std::chrono::sys_days> sysDaysFromString(const std::string& s) {
	using namespace std::literals;
	using namespace std::chrono;

	std::stringstream ss{ s };
	std::chrono::sys_days tt;
	ss >> std::chrono::parse("%F"s, tt);
	if (!ss.fail()) {
		return { tt };
	}
	return std::nullopt;
}

namespace sqlite_orm {

	/**
	 *  First of all is a type_printer template class.
	 *  It is responsible for sqlite type string representation.
	 *  We want SysDays to be `TEXT` so let's just derive from
	 *  text_printer. Also there are other printers: real_printer and
	 *  integer_printer. We must use them if we want to map our type to `REAL` (double/float)
	 *  or `INTEGER` (int/long/short etc) respectively.
	 */
	template<>
	struct type_printer<std::chrono::sys_days> : public text_printer {};

	/**
	 *  This is a binder class. It is used to bind c++ values to sqlite queries.
	 *  Here we have to create sysday string representation and bind it as string.
	 *  Any statement_binder specialization must have `int bind(sqlite3_stmt*, int, const T&)` function
	 *  which returns bind result. Also you can call any of `sqlite3_bind_*` functions directly.
	 *  More here https://www.sqlite.org/c3ref/bind_blob.html
	 */
	template<>
	struct statement_binder<std::chrono::sys_days> {

		int bind(sqlite3_stmt* stmt, int index, const std::chrono::sys_days& value) const {
			return statement_binder<std::string>().bind(stmt, index, sysDaysToString(value));
		}
	};

	/**
	 *  field_printer is used in `dump` and `where` functions. Here we have to create
	 *  a string from mapped object.
	 */
	template<>
	struct field_printer<std::chrono::sys_days> {
		std::string operator()(const std::chrono::sys_days& t) const {
			return sysDaysToString(t);
		}
	};

	/**
	 *  This is a reverse operation: here we have to specify a way to transform string received from
	 *  database to our sysdays object. Here we call `sysDaysFromString` and throw `std::runtime_error` if it returns null.
	 *  Every `row_extractor` specialization must have `extract(const char*)`, `extract(sqlite3_stmt *stmt, int columnIndex)`
	 *	and `extract(sqlite3_value* value)`
	 *  functions which return a mapped type value.
	 */
	template<>
	struct row_extractor<std::chrono::sys_days> {
		std::chrono::sys_days extract(const char* row_value) const {
			if (row_value) {
				auto sd = sysDaysFromString(row_value);
				if (sd) {
					return sd.value();
				}
				else {
					throw std::runtime_error("incorrect date string (" + std::string(row_value) + ")");
				}
			}
			else {
				// ! row_value
				throw std::runtime_error("incorrect date string (nullptr)");
			}
		}

		std::chrono::sys_days extract(sqlite3_stmt* stmt, int columnIndex) const {
			auto str = sqlite3_column_text(stmt, columnIndex);
			return this->extract((const char*)str);
		}
		std::chrono::sys_days extract(sqlite3_value* row_value) const {
			auto characters = (const char*)(sqlite3_value_text(row_value));
			return extract(characters);
		}
	};
}

namespace sql = sqlite_orm;


using namespace std::chrono;

namespace skribbl
{

	struct User {
		int m_id;
		std::string m_username;
		std::string m_password;
		int m_nrGamesPlayed;
	};

	struct GameHistory {
		int m_id;
		int m_id_player;
		int m_id_game;
		int m_points;
	};

	struct Games {
		int m_id;
		std::chrono::sys_days m_date;
	};

	struct Words {
		int m_id;
		std::string m_word;
	};

	inline auto CreateDatabase(const std::string& filename)
	{
		return sql::make_storage(
			filename,
			sql::make_table(
				"Users",
				sql::make_column("id", &User::m_id, sql::primary_key().autoincrement()),
				sql::make_column("username", &User::m_username),
				sql::make_column("password", &User::m_password),
				sql::make_column("nrGamesPlayed", &User::m_nrGamesPlayed)
			),
			sql::make_table(
				"Game",
				sql::make_column("id", &Games::m_id, sql::primary_key().autoincrement()),
				sql::make_column("date", &Games::m_date)
			),
			sql::make_table(
				"GamesHistory",
				sql::make_column("id", &GameHistory::m_id, sql::primary_key().autoincrement()),
				sql::make_column("user", &GameHistory::m_id_player),
				sql::make_column("game", &GameHistory::m_id_game),
				sql::make_column("points", &GameHistory::m_points),
				sql::foreign_key(&GameHistory::m_id_player).references(&User::m_id),
				sql::foreign_key(&GameHistory::m_id_game).references(&Games::m_id)
			),
			sql::make_table(
				"Words",
				sql::make_column("id", &Words::m_id, sql::primary_key().autoincrement()),
				sql::make_column("word", &Words::m_word)
			)
		);
	}

	using Storage = decltype(CreateDatabase(""));

	class Database
	{
	public:

		//Database();

		bool Initialize(const std::string& fileName);

		bool CheckUserExists(const std::string& username);
		void CreateNewUser(const std::string& username, const std::string& password);

		std::optional<User> AuthenticateUser(const std::string& username, const std::string& password);

		int GetNumberOfWords();
		std::string GetWord(int id);

		void AddGame(year_month_day date);
		year_month_day CurrentDate();

		void AddGameHistory(const std::vector<std::pair<std::string, int16_t>>& players);
		std::vector<std::tuple<int, std::string>> GetGameHistory(const std::string& username);

		void AddCustomWordToDatabase(const std::string& word);

	private:
		void PopulateStorage(const std::string& fileName);

		const std::string kDbFile{ "database.sqlite" };

		Storage m_db = CreateDatabase(kDbFile);
	};
}
