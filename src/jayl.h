#ifndef _JAYL_H
#define _JAYL_H

#include <string>
#include <vector>

namespace Jayl {
  typedef std::string String;

  template <typename T>
  struct Vector : std::vector<T> {};

  enum Operator {};

  struct Id;
  struct Sentence;
  struct Word;

  struct AccessExpression;
  struct CallExpression;
  struct CastExpression;
  struct ConstExpression;
  struct Expression;
  struct FrameExpression;
  struct LambdaExpression;
  struct NullExpression;
  struct OperationExpression;
  struct PositionExpression;
  struct PostifxExpression;
  struct ScalarExpression;
  struct SizeofExpression;
  struct TernaryExpression;
  struct ThisExpression;
  struct TypeofExpression;
  struct UnaryExpression;

  struct Statement;
  struct ExpressionStatement;
  struct LabelStatement;
  struct CompoundStatement;
  struct SelectStatement;
  struct IterStatement;
  struct ForStatement;
  struct JumpStatement;
  struct ReturnStatement;
  struct DeclarationStatement;

  struct Type;
  struct UserType;
  struct TypeSpecifier;
}

#endif /* _JAYL_H */
