CREATE TABLE users (
    id SERIAL PRIMARY KEY,
    name TEXT UNIQUE NOT NULL,
    created_at TIMESTAMPTZ NOT NULL DEFAULT now()
);

CREATE TABLE game_results (
    id SERIAL PRIMARY KEY,
    x_player_id INTEGER NOT NULL REFERENCES users(id),
    o_player_id INTEGER NOT NULL REFERENCES users(id),
    winner_id INTEGER REFERENCES users(id),
    played_at TIMESTAMPTZ NOT NULL DEFAULT now()
);