const getPalgoHandler = (wasmModule, palgo) => {
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
      const namedRecords = await palgo.wrapper(wasmModule, days, muscles);
      res.status(200).json(namedRecords);
      return;
    } catch (error) {
      res.status(400).json({message: error.message});
      return;
    }
  };
};

(async () => {
  const factory = require('./output.js');
  const wasmModule = await factory();

  const palgo = require('./palgo.js');

  const express = require('express');
  const app = express();
  const port = process.env.PORT || 8080;

  app.use(express.json())
  app.post('/api', getPalgoHandler(wasmModule, palgo));

  app.listen(port, () => {
    console.log("Listening on port " + port + "...");
  });
})();
