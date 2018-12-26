const { Router } = require('express');
const {
  configure,
  getCode,
  getAllCodes,
  setCode,
  setAllCodes,
  saveCode,
  saveAllCodes,
  stepCode,
  writeCsv,
  readCsv,
} = require('../util/utils');

const api = Router();

api.get('/configure', async (req, res) => {
  try {
    await configure();
    res.sendStatus(201);
  } catch (e) {
    res.status(400).send(e);
  }
});

api.get('/getCode', async (req, res) => {
  const { level } = req.query;
  try {
    const code = await getCode(+level);
    res.status(200).send({ code });
  } catch (e) {
    res.status(400).send(e);
  }
});

api.get('/getAllCodes', async (req, res) => {
  try {
    const codes = await getAllCodes();
    res.status(200).send({ codes });
  } catch (e) {
    res.status(400).send(e);
  }
});

api.post('/setCode', async (req, res) => {
  const { level, code } = req.body;
  try {
    await setCode(+level, +code);
    res.sendStatus(201);
  } catch (e) {
    res.status(400).send(e);
  }
});

api.post('/setAllCodes', async (req, res) => {
  const { codes } = req.body;
  try {
    await setAllCodes(codes);
    res.sendStatus(201);
  } catch (e) {
    res.status(400).send(e);
  }
});

api.post('/saveCode', async (req, res) => {
  const { level, code } = req.body;
  try {
    await saveCode(+level, +code);
    res.sendStatus(201);
  } catch (e) {
    res.status(400).send(e);
  }
});

api.post('/saveAllCodes', async (req, res) => {
  const { codes } = req.body;
  try {
    await saveAllCodes(codes);
    res.sendStatus(201);
  } catch (e) {
    res.status(400).send(e);
  }
});

api.post('/stepCode', async (req, res) => {
  const { level, direction } = req.body;
  try {
    await stepCode(+level, +direction);
    res.sendStatus(201);
  } catch (e) {
    res.status(400).send(e);
  }
});

api
  .route('/defaults')
  .get(async (req, res) => {
    const { channel } = req.query;
    try {
      const defaults = await readCsv(channel, 'defaults');
      res.status(200).send({ defaults });
    } catch (e) {
      res.status(404).send(e);
    }
  })
  .post(async (req, res) => {
    const { channel, defaults } = req.body;
    try {
      await writeCsv(channel, defaults, null, 'defaults');
      res.sendStatus(201);
    } catch (e) {
      res.status(400).send(e);
    }
  });

api
  .route('/current')
  .post(async (req, res) => {
    const { channel, values, unit } = req.body;
    try {
      await writeCsv(channel, values, unit, 'values');
      res.sendStatus(201);
    } catch (e) {
      res.status(400).send(e);
    }
  })
  .get(async (req, res) => {
    const { unit } = req.query;
    const data = [];
    const inner = async i => {
      try {
        const current = await readCsv(i, 'values', unit);
        data.push(current);
      } catch (e) {
        data.push(null);
      }
      if (i < 5) await inner(i + 1);
    };
    await inner(1);
    res.status(200).send({ data });
  });

module.exports = api;
