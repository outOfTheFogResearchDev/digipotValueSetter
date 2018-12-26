const express = require('express');
const session = require('express-session');
const bodyParser = require('body-parser');
const {
  promises: { writeFile, readFile, stat, mkdir },
} = require('fs');
const csvWrite = require('csv-stringify');
const csvRead = require('csv-parse');

const app = express();

app.disable('x-powered-by');

app.use(
  session({
    secret: 'not the secret',
    resave: false,
    saveUninitialized: false,
    cookie: {
      maxAge: 3600000, // one hour
    },
  })
);

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));

app.use(express.static(`${__dirname}/../client/dist`));

const defaultCodeHelper = [12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1].map(num => [num, (12 - num + 9) * 10]);

const csvFolderLocation = './server/local';
const csvLocation = (channel, unit, type) =>
  `${csvFolderLocation}/${unit ? `unit${unit}_` : ''}channel${channel}_${type}.csv`;

const writeCsv = async (channel, codes, unit, type) => {
  codes.sort((x, y) => +y[0] - +x[0]);
  const csv = await new Promise(resolve => csvWrite(codes, (err, data) => resolve(data)));
  // if the local folder doesnt exist, make it
  try {
    await stat(csvFolderLocation);
  } catch (e) {
    await mkdir(csvFolderLocation);
  }
  await writeFile(csvLocation(channel, unit, type), csv);
};

const readCsv = async (channel, type, unit = null) => {
  // check to see if that channel has a default file
  try {
    await stat(csvLocation(channel, unit, type));
  } catch (e) {
    throw new Error("That channel doesn't exist or doesn't have a default record");
  }
  const csv = await readFile(csvLocation(channel, unit, type), 'utf8');
  return new Promise(resolve => csvRead(csv, (err, data) => resolve(data)));
};

app.get('/api/configure', (req, res) => res.sendStatus(201));

app.get('/api/getCode', (req, res) => res.status(200).send({ code: (12 - req.level + 9) * 10 }));

app.get('/api/getAllCodes', (req, res) => res.status(200).send({ codes: defaultCodeHelper }));

app.post('/api/setCode', (req, res) => res.sendStatus(201));

app.post('/api/setAllCodes', (req, res) => res.sendStatus(201));

app.post('/api/saveCode', (req, res) => res.sendStatus(201));

app.post('/api/saveAllCodes', (req, res) => res.sendStatus(201));

app.post('/api/stepCode', (req, res) => res.sendStatus(201));

app
  .route('/api/defaults')
  .get((req, res) => res.status(200).send({ defaults: defaultCodeHelper }))
  .post((req, res) => res.sendStatus(201));

app
  .route('/api/current')
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

app.listen(3000, () => console.log('Listening on port: ', 3000)); // eslint-disable-line no-console
