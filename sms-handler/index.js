const https = require('https');

exports.message = (req, res) => {
  console.log(`Twilio message: ${req.body.Body}`);
  if (req.body.Body == 'on') {
    console.log('Switching lights on');
    https.get('https://cloud.arest.io/h6n3uu/digital/5/1');
  }else if (req.body.Body == 'off') {
    console.log('Switching lights off');
    https.get('https://cloud.arest.io/h6n3uu/digital/5/0');
  }
  res.status(200).end();
};
