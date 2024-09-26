const getPalgoHandler = (wasmModule, palgo, db, data) => {
  return async (req, res) => {
    const days = req.body.days;
    if (days === undefined) {
      res.status(400).json({message: "Days are not specified."});
      return;
    }

    const muscles = req.body.muscles;
    if (muscles === undefined) {
      res.status(400).json({message: "Muscles are not specified."});
      return;
    }

    const musclesList = [];
    for (let i = 0; i < muscles.length; i++) {
      if (muscles[i].name === undefined) {
        res.status(400).json({message: "Muscle name is not specified."});
        return;
      }
      if (muscles[i].maxmg === undefined) {
        res.status(400).json({message: "Muscle daily maximum is not specified."});
        return;
      }
      if (muscles[i].minw === undefined) {
        res.status(400).json({message: "Muscle weekly minimum is not specified."});
        return;
      }
      musclesList.push(new palgo.Muscle(
        muscles[i].name,
        muscles[i].maxmg,
        muscles[i].minw,
      ));
    }

    try {
      const id = req.session.id;
      const namedRecords = await palgo.wrapper(wasmModule, days, muscles);
      const result = await data.insertNamedRecords(db, id, namedRecords);

      res.status(200).json(namedRecords);
      return;
    } catch (error) {
      res.status(500).json({message: error.message});
      return;
    }
  };
};

const getHistoryHandler = (db, data) => {
  return async (req, res) => {
    const id = req.session.id;
    const result = await data.getNamedRecords(db, id);
    if (result === undefined) {
      res.status(404).json({message: "No named records found."});
      return;
    }

    res.status(200).json(result);
    return;
  }
};

const getSetCookie = (db, data) => {
  return async (req, res, next) => {
    if (req.session.id === undefined
      || await data.getUser(db, req.session.id) === undefined) {
      req.session.id = await data.insertUser(db);
    }
    next();
  }
}

(async () => {
  const factory = require('./output.js');
  const wasmModule = await factory();

  const palgo = require('./palgo.js');

  // Setup express and cookies
  const express = require('express');
  const app = express();
  const port = process.env.PORT || 8080;
  const cookieSession = require('cookie-session');
  app.use(cookieSession({
    name: 'session',
    keys: ['id'],
  }));

  // Setup sqlite
  const sqlite = require('sqlite');
  const sqlite3 = require('sqlite3');
  const db = await sqlite.open({
    filename: './database.sqlite',
    driver: sqlite3.Database,
  });
  const data = require('./data.js');
  data.setupDatabase(db);

  app.use(express.json())
  app.use(getSetCookie(db, data));
  app.post('/api/new', getPalgoHandler(wasmModule, palgo, db, data));
  app.get('/api/old', getHistoryHandler(db, data));

  app.listen(port, () => {
    console.log("Listening on port " + port + "...");
})();
