const angle = require('./');

describe('angle', () => {
  describe('add', () => {
    it('should add 0 numbers', () => {
      expect(angle.add()).toBe(0);
    });
    it('should add 3 numbers', () => {
      expect(angle.add(1, 2, 3)).toBe(6);
    });
    it('should throw on non-number', () => {
      expect(() => angle.add(1, 2, '3')).toThrowError('args should be integers.');
    });
  });
});
