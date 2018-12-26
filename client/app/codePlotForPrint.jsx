import React from 'react';
import styled from 'styled-components';

const LevelForm = styled.form`
  padding: 4px 5px;
  margin: 11px 0;
  margin-left: 20px;
  margin-right: 25px;
  box-sizing: border-box;
  display: inline-block;
`;

const LevelText = styled.label`
  display: inline-block;
  font-size: 125%;
`;

const LevelValue = styled.input`
  display: inline-block;
  width: 40px;
  margin-left: 15px;
  transform: scale(1.45);
`;

// eslint-disable-next-line react/prop-types
export default ({ level, code }) => (
  <LevelForm>
    <LevelText>Level {(+level < 10 ? '0' : '') + level}:</LevelText>
    <LevelValue type="number" value={code} readOnly />
  </LevelForm>
);
