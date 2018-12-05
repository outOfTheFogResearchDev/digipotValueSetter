import React, { Fragment, Component } from 'react';
import { get } from 'axios';
import CodePlot from './codePlot';

export default class extends Component {
  constructor(props) {
    super(props);
    this.state = { codes: {} };
  }

  async componentDidMount() {
    try {
      const codes = await get('/api/getAllCodes');
      this.setState({ codes });
    } catch (e) {
      alert(e); // eslint-disable-line no-alert
    }
  }

  render() {
    const { codes } = this.state;
    return (
      <Fragment>
        {Object.entries(codes).map(([channel, code]) => (
          <CodePlot channel={channel} code={code} />
        ))}
      </Fragment>
    );
  }
}
