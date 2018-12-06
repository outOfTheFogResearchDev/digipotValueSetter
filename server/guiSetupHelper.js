const express = require('express');
const session = require('express-session');
const bodyParser = require('body-parser');

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

app.use(express.static(`${__dirname}/../client/dist/`));

const defaultCodeHelper = [12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1].map(num => [num, (12 - num + 9) * 10]);

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

app.post('/api/current', (req, res) => res.sendStatus(201));

app.listen(3000, () => console.log('Listening on port: ', 3000)); // eslint-disable-line no-console
