#include "Routing.h"
using namespace skribbl;

skribbl::Routing::Routing(skribbl::Database& db) : m_db(db)
{
	m_ur = std::make_unique<UniqueRandom<std::string>>(kmaxGamesSupported);
}

void skribbl::Routing::Run()
{
	/*
	-----------------------------------------------------------------------------------------------------------------

										-> HTTP RESPONSE STATUS CODES <-

		~This comment is provided for a better understanding of the standard/custom responses that the code uses~

		~For the sake of easier reading, the template will look something like:~
		Type of response:
			[response status code]
			*A small description.

		_none_ is used for no code


		Information:

			_none_

		Successful:

			200
			*Ok. The request succeeded.

			201
			*Created. The request succeded and a new resorce was created.

			204
			*There is no content to send for this request, but the headers may be usefull.

		Redirection messages:

			_none_

		Client error:

			401
			*Unauthorized. Although the HTTP standard specifies "unauthorized", semantically
						   this response means "unauthenticated".
			*(ex: invalid username or password)

			404
			*Not Found. The server cannot find the requested resource.

			409
			*Conflict. Request conflicts with the current state of the server.
			*(ex: a lobby is full)

		Server error:

			503:
			*Service Unavailable. The server is not ready to handle the request.
			*(ex: server full; maximum number of games that can hold is reached)

		Custom:

			_none_


		~For more you can go to:
		https://developer.mozilla.org/en-US/docs/Web/HTTP/Status#browser_compatibility
		https://http.dev/status
		~

	-----------------------------------------------------------------------------------------------------------------
	*/

	CROW_ROUTE(m_app, "/login")
		.methods(crow::HTTPMethod::PUT)(
			[this](const crow::request& req)
			{
				return Login(req);
			}
	);


	CROW_ROUTE(m_app, "/register")
		.methods(crow::HTTPMethod::PUT)(
			[this](const crow::request& req)
			{
				return Register(req);
			}
	);

	CROW_ROUTE(m_app, "/create_lobby")
		.methods(crow::HTTPMethod::PUT)(
			[this](const crow::request& req)
			{
				return CreateLobby(req);
			}
	);

	CROW_ROUTE(m_app, "/join_lobby")
		.methods(crow::HTTPMethod::PUT)(
			[this](const crow::request& req)
			{
				return JoinLobby(req);
			}
	);

	CROW_ROUTE(m_app, "/start")
		.methods(crow::HTTPMethod::PUT)(
			[this](const crow::request& req)
			{
				return StartGame(req);
			}
	);

	CROW_ROUTE(m_app, "/reset")
		.methods(crow::HTTPMethod::PUT)(
			[this](const crow::request& req)
			{
				return ResetGame(req);
			});

	CROW_ROUTE(m_app, "/drawing_player")
		.methods(crow::HTTPMethod::GET)(
			[this](const crow::request& req)
			{
				return GetDrawingPlayer(req);
			}
	);

	CROW_ROUTE(m_app, "/get_word")
		.methods(crow::HTTPMethod::GET)(
			[this](const crow::request& req)
			{
				return GetWord(req);
			}
	);

	CROW_ROUTE(m_app, "/get_hint")
		.methods(crow::HTTPMethod::GET)(
			[this](const crow::request& req)
			{
				return GetHint(req);
			}
	);

	CROW_ROUTE(m_app, "/send_answer")
		.methods(crow::HTTPMethod::PUT)(
			[this](const crow::request& req)
			{
				return ProcessAnswer(req);
			}
	);

	CROW_ROUTE(m_app, "/get_answers")
		.methods(crow::HTTPMethod::GET)(
			[this](const crow::request& req)
			{
				return GetAnswers(req);
			}
	);

	CROW_ROUTE(m_app, "/send_drawing")
		.methods(crow::HTTPMethod::PUT)(
			[this](const crow::request& req)
			{
				return ProcessDrawing(req);
			}
	);

	CROW_ROUTE(m_app, "/get_drawing")
		.methods(crow::HTTPMethod::GET)(
			[this](const crow::request& req)
			{
				return GetDrawing(req);
			}
	);

	CROW_ROUTE(m_app, "/get_time")
		.methods(crow::HTTPMethod::GET)(
			[this](const crow::request& req)
			{
				return GetTime(req);
			}
	);

	CROW_ROUTE(m_app, "/remove")
		.methods(crow::HTTPMethod::PUT)(
			[this](const crow::request& req)
			{
				return RemovePlayer(req);
			}
	);

	CROW_ROUTE(m_app, "/leaderboard")
		.methods(crow::HTTPMethod::GET)(
			[this](const crow::request& req)
			{
				return GetGameLeaderboard(req);
			}
	);

	CROW_ROUTE(m_app, "/game_state")
		.methods(crow::HTTPMethod::GET)(
			[this](const crow::request& req)
			{
				return GetGameState(req);
			}
	);

	CROW_ROUTE(m_app, "/players")
		.methods(crow::HTTPMethod::GET)(
			[this](const crow::request& req)
			{
				return GetGamePlayers(req);
			}
	);

	CROW_ROUTE(m_app, "/games_history")
		.methods(crow::HTTPMethod::GET)(
			[this](const crow::request& req)
			{
				return GetGamesHistory(req);
			}
	);
	CROW_ROUTE(m_app, "/custom_word")
		.methods(crow::HTTPMethod::PUT)(
			[this](const crow::request& req)
			{
				return AddCustomWord(req);
			}
	);

	CROW_ROUTE(m_app, "/custom_words_count")
		.methods(crow::HTTPMethod::GET)(
			[this](const crow::request& req)
			{
				return GetCustomWordsCount(req);
			}
	);

	m_app.port(18080).multithreaded().run();
}

crow::response Routing::JoinLobby(const crow::request& req)
{
	std::lock_guard lock{ m_mutex };

	std::string lobbyCode = req.url_params.get("lobbyCode");
	if (m_games.find(lobbyCode) == m_games.end())
		return crow::response(404, "Lobby not found!");

	if (m_games[lobbyCode]->HasStarted())
		return crow::response(404, "Game started!");

	std::string playerName = req.url_params.get("playerName");
	if (m_games[lobbyCode]->AddPlayer(playerName))
	{
		return crow::response(201);
	}
	else
		return crow::response(409, "Lobby full!");
}

crow::response Routing::StartGame(const crow::request& req)
{
	std::string lobbyCode = req.url_params.get("lobbyCode");
	if (m_games.find(lobbyCode) == m_games.end())
		return crow::response(404, "Game not found!");

	m_games[lobbyCode]->Start();

	return crow::response(204);
}

crow::response Routing::ResetGame(const crow::request& req)
{
	std::string lobbyCode = req.url_params.get("lobbyCode");
	if (m_games.find(lobbyCode) == m_games.end())
		return crow::response(404, "Game not found!");

	m_games[lobbyCode]->Restart();
	return crow::response(204);
}

crow::response Routing::CreateLobby(const crow::request& req)
{
	std::lock_guard lock{ m_mutex };

	if (m_games.size() == kmaxGamesSupported)
		return crow::response(503, "Server full!");

	std::string lobbyCode = m_ur->GetValue();

	m_games[lobbyCode] = IGame::Factory(m_db);

	std::string playerName = req.url_params.get("playerName");
	m_games[lobbyCode]->AddPlayer(playerName);

	return crow::response(201, lobbyCode);
}

crow::response Routing::GetGameLeaderboard(const crow::request& req)
{
	std::string lobbyCode = req.url_params.get("lobbyCode");
	if (m_games.find(lobbyCode) == m_games.end())
		return crow::response(404, "Game not found!");

	std::vector<crow::json::wvalue> results;
	for (const auto& comp : m_games[lobbyCode]->GetLeaderboard())
	{
		results.push_back(crow::json::wvalue{
			{"playerName", comp.first},
			{"score", comp.second}
			});
	}
	return crow::json::wvalue{ results };
}

crow::response Routing::RemovePlayer(const crow::request& req)
{
	std::lock_guard lock{ m_mutex };

	std::string lobbyCode = req.url_params.get("lobbyCode");
	if (m_games.find(lobbyCode) == m_games.end())
		return crow::response(404, "Game not found!");

	std::string playerName = req.url_params.get("playerName");

	m_games[lobbyCode]->RemovePlayer(playerName);

	if (m_games[lobbyCode]->GetPlayers().size() == 0)
	{
		auto it = m_games.find(lobbyCode);
		m_games.erase(it);
		m_ur->RemoveValue(std::stoi(lobbyCode));
	}
	return crow::response(204);
}

crow::response Routing::ProcessAnswer(const crow::request& req)
{
	std::lock_guard lock{ m_mutex };

	std::string lobbyCode = req.url_params.get("lobbyCode");
	if (m_games.find(lobbyCode) == m_games.end())
		return crow::response(404, "Game not found!");

	std::string playerName = req.url_params.get("playerName");
	std::string answer = req.url_params.get("answer");


	crow::json::wvalue jsonResponse;
	jsonResponse["playerName"] = playerName;
	if (m_games[lobbyCode]->VerifyAnswer(playerName, answer))
	{
		answer = "Has guessed the word!";
		jsonResponse["hasGuessed"] = true;
	}
	else
		jsonResponse["hasGuessed"] = false;
	m_games[lobbyCode]->AddAnswer(playerName, answer);
	return crow::response(200, jsonResponse);
}

crow::response Routing::ProcessDrawing(const crow::request& req)
{
	std::string lobbyCode = req.url_params.get("lobbyCode");
	if (m_games.find(lobbyCode) == m_games.end())
		return crow::response(404, "Game not found!");

	auto drawing = req.url_params.get("drawing");
	m_games[lobbyCode]->SetDrawing(req.url_params.get("drawing"));
	return crow::response(200);
}

crow::response Routing::GetDrawing(const crow::request& req)
{
	std::string lobbyCode = req.url_params.get("lobbyCode");
	if (m_games.find(lobbyCode) == m_games.end())
		return crow::response(404, "Game not found!");

	std::string drawingStr = m_games[lobbyCode]->GetDrawing();
	return crow::response(200, drawingStr);
}

crow::response Routing::GetWord(const crow::request& req)
{
	std::string lobbyCode = req.url_params.get("lobbyCode");
	if (m_games.find(lobbyCode) == m_games.end())
		return crow::response(404, "Game not found!");

	std::string playerName = req.url_params.get("playerName");

	return crow::response(200, m_games[lobbyCode]->GetWord());
}

crow::response Routing::GetAnswers(const crow::request& req)
{
	std::string lobbyCode = req.url_params.get("lobbyCode");
	if (m_games.find(lobbyCode) == m_games.end())
		return crow::response(404, "Game not found!");


	std::vector<crow::json::wvalue> answers_json;
	auto answers = m_games[lobbyCode]->GetAnswers();
	if (answers.size() == 0)
		return crow::response(204);
	for (const auto& answer : answers)
	{
		answers_json.push_back(crow::json::wvalue{
			{"playerName", answer.playerName},
			{"answer", answer.answer}
			});
	}
	return crow::json::wvalue{ answers_json };
}

crow::response Routing::GetDrawingPlayer(const crow::request& req)
{
	std::string lobbyCode = req.url_params.get("lobbyCode");
	if (m_games.find(lobbyCode) == m_games.end())
		return crow::response(404, "Game not found!");

	return crow::response(200, m_games[lobbyCode]->GetDrawingPlayer());
}

crow::response Routing::GetGameState(const crow::request& req)
{
	std::string lobbyCode = req.url_params.get("lobbyCode");
	if (m_games.find(lobbyCode) == m_games.end())
		return crow::response(404, "Game not found!");

	return crow::response(200, m_games[lobbyCode]->GetState());
}

crow::response Routing::Login(const crow::request& req)
{
	std::lock_guard lock{ m_mutex };

	std::string username = req.url_params.get("username");
	std::string password = req.url_params.get("password");

	std::optional<skribbl::User> user = m_db.AuthenticateUser(username, password);
	if (user.has_value())
	{
		return crow::response(200, username);
	}
	else
	{
		return crow::response(401, "Invalid username or password!");
	}
}

crow::response Routing::Register(const crow::request& req)
{
	std::lock_guard lock{ m_mutex };

	std::string password = req.url_params.get("password");
	std::string username = req.url_params.get("username");

	if (m_db.CheckUserExists(username))
		return crow::response(409, "User already exists!");

	m_db.CreateNewUser(username, password);

	return crow::response(201, username);
}

crow::response Routing::GetTime(const crow::request& req)
{
	std::string lobbyCode = req.url_params.get("lobbyCode");
	if (m_games.find(lobbyCode) == m_games.end())
		return crow::response(404, "Game not found!");

	return crow::response(200, std::to_string(m_games[lobbyCode]->GetTime()));
}

crow::response Routing::GetHint(const crow::request& req)
{
	std::string lobbyCode = req.url_params.get("lobbyCode");
	if (m_games.find(lobbyCode) == m_games.end())
		return crow::response(404, "Game not found!");
	
	std::vector<uint8_t> hints = m_games[lobbyCode]->GetHint();
	
	std::string hintsString = "";
	
	for (const auto& hint : hints)
	{
		hintsString += hint;
	}

	return crow::response(200, hintsString);
}

crow::response Routing::GetGamePlayers(const crow::request& req)
{
	std::string lobbyCode = req.url_params.get("lobbyCode");
	if (m_games.find(lobbyCode) == m_games.end())
		return crow::response(404, "Game not found!");

	std::vector<std::pair<std::string, int16_t>> players = m_games[lobbyCode]->GetPlayers();

	std::vector<crow::json::wvalue> results;
	for (const auto& comp : players)
	{
		results.push_back(crow::json::wvalue{
			{"playerName", comp.first},
			{"score", comp.second}
			});
	}
	return crow::json::wvalue{ results };
}

crow::response skribbl::Routing::GetGamesHistory(const crow::request & req) const
{
	std::string username = req.url_params.get("username");

	std::vector<crow::json::wvalue> results;
	for (const auto& history : m_db.GetGameHistory(username))
	{
		results.push_back(crow::json::wvalue{
			{"playerScore", std::get<0>(history)},
			{"date", std::get<1>(history)}
			});
	}

	return crow::json::wvalue{ results };
}

crow::response Routing::GetCustomWordsCount(const crow::request& req)
{
	std::string lobbyCode = req.url_params.get("lobbyCode");
	if (m_games.find(lobbyCode) == m_games.end())
		return crow::response(404, "Game not found!");

	uint8_t number = m_games[lobbyCode]->GetCustomWordsCount();

	crow::json::wvalue jsonResponse;
	jsonResponse["numberCustomWords"] = number;

	return crow::response(200, jsonResponse);
}


crow::response Routing::AddCustomWord(const crow::request& req)
{
	std::string lobbyCode = req.url_params.get("lobbyCode");
	if (m_games.find(lobbyCode) == m_games.end())
		return crow::response(404, "Game not found!");

	std::string word = req.url_params.get("word");
	m_games[lobbyCode]->AddCustomWord(word);//in game

	return crow::response(201, "Added!");
}
