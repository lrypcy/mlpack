/**
 * @file alpha_dropout.hpp
 * @author Dakshit Agrawal
 *
 * Definition of the Alpha-Dropout class, which implements a regularizer that
 * randomly sets units to alpha-dash to prevent them from co-adapting and
 * makes an affine transformation so as to keep the mean and variance of
 * outputs at their original values.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */

#ifndef MLPACK_METHODS_ANN_LAYER_ALPHA_DROPOUT_HPP
#define MLPACK_METHODS_ANN_LAYER_ALPHA_DROPOUT_HPP

#include <mlpack/prereqs.hpp>

namespace mlpack {
namespace ann /** Artificial Neural Network. */ {

/**
 * The alpha - dropout layer is a regularizer that randomly with probability
 * 'ratio' sets input values to alphaDash. The alpha - dropout layer is mostly
 * used for SELU activation function where successive layers don't have same
 * mean and variance.
 *
 * For more information, see the following.
 *
 * @code
 * @article{Klambauer2017,
 *   author  = {Gunter Klambauer and Thomas Unterthiner and
 *              Andreas Mayr},
 *   title   = {Self-Normalizing Neural Networks},
 *   journal = {Advances in Neural Information Processing Systems},
 *   year    = {2017}
 * }
 * }
 * @endcode
 *
 * @tparam InputDataType Type of the input data (arma::colvec, arma::mat,
 *         arma::sp_mat or arma::cube).
 * @tparam OutputDataType Type of the output data (arma::colvec, arma::mat,
 *         arma::sp_mat or arma::cube).
 */
template <typename InputDataType = arma::mat,
          typename OutputDataType = arma::mat>
class AlphaDropout
{
 public:
  /**
   * Create the Alpha_Dropout object using the specified ratio.
   *
   * @param ratio The probability of setting a value to alphaDash.
   * @param alphaDash The dropout scaling parameter.
   */
  AlphaDropout(const double ratio = 0.5,
               const double alphaDash = -alpha * lambda);

  /**
   * Ordinary feed forward pass of the alpha_dropout layer.
   *
   * @param input Input data used for evaluating the specified function.
   * @param output Resulting output activation.
   */
  template<typename eT>
  void Forward(const arma::Mat<eT>&& input, arma::Mat<eT>&& output);

  /**
   * Ordinary feed backward pass of the alpha_dropout layer.
   *
   * @param input The propagated input activation.
   * @param gy The backpropagated error.
   * @param g The calculated gradient.
   */
  template<typename eT>
  void Backward(const arma::Mat<eT>&& /* input */,
                arma::Mat<eT>&& gy,
                arma::Mat<eT>&& g);

  //! Get the input parameter.
  InputDataType const& InputParameter() const { return inputParameter; }
  //! Modify the input parameter.
  InputDataType& InputParameter() { return inputParameter; }

  //! Get the output parameter.
  OutputDataType const& OutputParameter() const { return outputParameter; }
  //! Modify the output parameter.
  OutputDataType& OutputParameter() { return outputParameter; }

  //! Get the detla.
  OutputDataType const& Delta() const { return delta; }
  //! Modify the delta.
  OutputDataType& Delta() { return delta; }

  //! The value of the deterministic parameter.
  bool Deterministic() const { return deterministic; }
  //! Modify the value of the deterministic parameter.
  bool& Deterministic() { return deterministic; }

  //! The probability of setting a value to alphaDash.
  double Ratio() const { return ratio; }

  //! Value to be multiplied with x for affine transformation.
  double A() const { return a; }

  //! Value to be added to a*x for affine transformation.
  double B() const { return b; }

  //! Value of alphaDash.
  double AlphaDash() const {return alphaDash; }

  //! Get the mask.
  OutputDataType const& Mask() const {return mask;}

  //! Modify the probability of setting a value to alphaDash. As
  //! 'a' and 'b' depend on 'ratio', modify them as well.
  void Ratio(const double r)
  {
    ratio = r;
    a = pow((1 - ratio) * (1 + ratio * pow(alphaDash, 2)), -0.5);
    b = -a * alphaDash * ratio;
  }

  /**
   * Serialize the layer.
   */
  template<typename Archive>
  void serialize(Archive& ar, const unsigned int /* version */);

 private:
  //! Locally-stored delta object.
  OutputDataType delta;

  //! Locally-stored input parameter object.
  InputDataType inputParameter;

  //! Locally-stored output parameter object.
  OutputDataType outputParameter;

  //! Locally-stored mast object.
  OutputDataType mask;

  //! The probability of setting a value to aplhaDash.
  double ratio;

  //! The low variance value of SELU activation function.
  double alphaDash;

  //! If true dropout and scaling is disabled, see notes above.
  bool deterministic;

  //! Value of alpha for normalized inputs (taken from SELU).
  static constexpr double alpha = 1.6732632423543772848170429916717;

  //! Value of lambda for normalized inputs (taken from SELU).
  static constexpr double lambda = 1.0507009873554804934193349852946;

  //! Value to be multiplied with x for affine transformation.
  double a;

  //! Value to be added to a*x for affine transformation.
  double b;
}; // class AlphaDropout

} // namespace ann
} // namespace mlpack

// Include implementation.
#include "alpha_dropout_impl.hpp"

#endif
