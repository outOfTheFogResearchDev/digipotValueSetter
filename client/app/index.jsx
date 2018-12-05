import React, { Fragment, Component } from 'react';
import { get, post } from 'axios';
import CodePlot from './codePlot';

export default class extends Component {
  constructor(props) {
    super(props);
    this.state = { codes: [], channel: 1 };
  }

  async componentDidMount() {
    await this.getAllCodes();
  }

  async getAllCodes() {
    try {
      const {
        data: { codes },
      } = await get('/api/getAllCodes');
      this.setState({ codes });
    } catch (e) {
      alert(e); // eslint-disable-line no-alert
    }
  }

  async setDefaults() {
    const { channel, codes } = this.state;
    try {
      await post('/api/defaults', { channel, defaults: codes });
    } catch (e) {
      alert(e); // eslint-disable-line no-alert
    }
  }

  async applyDefaults() {
    const { channel } = this.state;
    try {
      const {
        data: { codes },
      } = await get('/api/defaults', { params: { channel } });
      await post('/api/setAllCodes', { codes });
      await post('/api/saveAllCodes', { codes });
      await this.getAllCodes();
    } catch (e) {
      alert(e); // eslint-disable-line no-alert
    }
  }

  render() {
    const { codes } = this.state;
    return (
      <Fragment>
        {codes.map(([channel, code]) => (
          <CodePlot channel={channel} code={code} />
        ))}
      </Fragment>
    );
  }
}
