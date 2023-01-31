import { StateArgos } from './StateArgos.jsx';
import Button from 'semantic-ui-react';
import './Argos.css';
import { shallow } from 'enzyme';

// describe('Button', () => {
  const startMission = async () => {
  };

  function MySpy(){
    this.calls = 0;
  }

  MySpy.prototype.fn = function () {
    return () => this.calls++;
  }

  it('Test Button component', () => {
    const mySpy = new MySpy();
    const mockCallBack = mySpy.fn();
  
    const button = shallow((<Button onClick={mockCallBack}>Ok!</Button>));
  
    button.find('button').simulate('click');
    expect(mySpy.calls).toEqual(1);
  });


 test('should be defined', () => {
   expect(Button).toBeDefined();
 });

// test('clicking the button calls startMission', ()=>{
//   const mockCallBack = sinon.spy();
//   const button = shallow((<Button onClick={mockCallBack}>Ok!</Button>))
//   button.find('button').simulate('click');
//   expect(mockCallBack).toHaveProperty('callCount',1);


// const demarrer = StateArgos(
//     <Button name='button test' handleClick={startMission}/>
// );
// demarrer.simulate('click');
// expect(startMission).toHaveBeenCalled();
// });