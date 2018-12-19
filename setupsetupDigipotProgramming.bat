git clone https://github.com/outOfTheFogResearchDev/digipotValueSetter
cd digipotValueSetter
mkdir config
echo exports.SECRET = `${Math.random()}`; > config/config.js
npm run setup