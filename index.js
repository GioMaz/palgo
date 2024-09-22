const palgoAlgo = async (module, N, M, maxmg, minw) => {

  const maxmgVec = new module.IntVec();
  maxmg.forEach(x => {
    maxmgVec.push_back(x);
  });

  const minwVec = new module.IntVec();
  minw.forEach(x => {
    minwVec.push_back(x);
  });

  const recordVec = module.palgo(N, M, maxmgVec, minwVec);
  const records = [];
  for (let i = 0; i < recordVec.size(); i++) {
    records.push(recordVec.get(i));
  }
  return records;
}

class NamedRecord {
  constructor(name, sets) {
    this.name = name;
    this.sets = sets;
  }
}

const palgoWrapper = async (module, days, muscles) => {
  const maxmg = [];
  const minw  = [];
  muscles.forEach(muscle => {
    maxmg.push(muscle.maxmg);
    minw.push(muscle.minw);
  });
  const records = await palgoAlgo(module, days, muscles.length, maxmg, minw);
  if (records.length == 0) {
    throw new Error("Input constraints are not satisfiable.");
  }

  const namedRecords = Array(days).fill().map(_ => []);
  records.forEach(record => {
    namedRecords[record.g].push(
      new NamedRecord(muscles[record.m].name, record.s)
    );
  });
  return namedRecords;
};

class Muscle {
  constructor(name, maxmg, minw) {
    this.name   = name;
    this.maxmg  = maxmg;
    this.minw   = minw;
  }
};

const createProgram = async () => {
  const days = 3;
  const muscles = [
    new Muscle("Petto",     12, 24),
    new Muscle("Schiena",   30, 24),
    new Muscle("Spalle",    8,  24),
    new Muscle("Gambe",     12, 12),
    new Muscle("Bicipiti",  12, 24),
    new Muscle("Tricipiti", 8,  12),
  ];

  try {
    const namedRecords = await palgoWrapper(days, muscles);
    console.log(namedRecords);
  } catch (error) {
    console.log(error);
  }
};

const palgoHandlerModule = module => {
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
      musclesList.push(new Muscle(
        muscles[i].name,
        muscles[i].maxmg,
        muscles[i].minw,
      ));
    }

    try {
      const namedRecords = await palgoWrapper(module, days, muscles);
      res.status(200).json(namedRecords);
      return;
    } catch (error) {
      res.status(400).json({message: error.message});
      return;
    }
  };
};

(async () => {
  const factory = require('./palgo.js');
  const module = await factory();

  const express = require('express');
  const app = express();
  const port = 8080;

  app.use(express.json())
  app.post('/api', palgoHandlerModule(module));

  app.listen(port, () => {
    console.log("Listening on port " + port + "...");
  });
})();
