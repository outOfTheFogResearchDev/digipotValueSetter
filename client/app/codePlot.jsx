import React, { Fragment, Component } from 'react';

export default class extends Component {
  constructor(props) {
    super(props);
    const { channel, code } = this.props; // eslint-disable-line react/prop-types
    this.state = { channel, code };
  }

  render() {
    const { channel, code } = this.state;
    return (
      <Fragment>
        Channel # {channel}: {code}
      </Fragment>
    );
  }
}
