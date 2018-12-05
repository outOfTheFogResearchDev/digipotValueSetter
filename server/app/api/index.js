const { Router } = require('express');
const { configure, getCode, getAllCodes, saveCode, setCode, stepCode, writeCsv, readCsv } = require('../util/utils');

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
  const { channel } = req.query;
  try {
    const code = await getCode(channel);
    res.status(200).send({ code });
  } catch (e) {
    res.status(400).send(e);
  }
});

api.get('/getAllCode', async (req, res) => {
  try {
    const codes = await getAllCodes();
    res.status(200).send({ codes });
  } catch (e) {
    res.status(400).send(e);
  }
});

api.post('/saveCode', async (req, res) => {
  const { channel, code } = req.body;
  try {
    await saveCode(channel, code);
    res.sendStatus(201);
  } catch (e) {
    res.status(400).send(e);
  }
});

api.post('/setCode', async (req, res) => {
  const { channel, code } = req.body;
  try {
    await setCode(channel, code);
    res.sendStatus(201);
  } catch (e) {
    res.status(400).send(e);
  }
});

api.post('/stepCode', async (req, res) => {
  const { channel, direction } = req.body;
  try {
    await stepCode(channel, direction);
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
      const defaults = await readCsv(channel);
      res.status(200).send({ defaults });
    } catch (e) {
      res.status(404).send(e);
    }
  })
  .post(async (req, res) => {
    const { channel, defaults } = req.body;
    try {
      await writeCsv(channel, defaults);
      res.sendStatus(201);
    } catch (e) {
      res.status(400).send(e);
    }
  });

module.exports = api;
