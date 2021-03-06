/*
 * main.cpp
 * 1 - Simple Function Evaluator
 * CAF - Exercise
 * Nils Schnabl
 * HAW Hamburg
 */

#include <iostream>
#include <string>
#include "caf/all.hpp"

using namespace std;
using namespace caf;

using calc_atom = atom_constant<atom("calc")>;

// this actor/method evaluates the required function
behavior functionEvaluator(event_based_actor* self, int a0, int a1, int a2, int a3, int a4) {

    double v0 = static_cast< double >(a0);
    double v1 = static_cast< double >(a1);
    double v2 = static_cast< double >(a2);
    double v3 = static_cast< double >(a3);
    double v4 = static_cast< double >(a4);

    return {
        [=](calc_atom, const double& request) -> double {
            cout << "--> inside actor 'functionEvaluator' <--" << endl;
            double result = 0.0;

            cout << "function evalution can be done, because the right atom is send." << endl;
            result = v0 * pow(request, 4) + v1 * pow(request, 3) + v2 * pow(request, 2) + v3 * request + v4;

            //self->quit();
            return result;
        }
    };
}

// this actor/method reads value X, sends X to the functionEvaluator, waits for result
// and then prints the result
void print_evaluator(event_based_actor* self, const actor& buddy) {
    double x = 0;
    cout << "Enter one Double-type-number: ";
    cin >> x;
    // sending X(Double)-parameter to our buddy
    self->sync_send(buddy, calc_atom::value, x).then(
        // wait for response
        [=](const double& result) {
            //printing result
            cout << "Result Y of the function evaluation: ";
            cout << result << endl;

            // reading next value of X, for this an actor of himself is created,
            // for handling the 'functionEvaluator'- actor another time
            spawn(print_evaluator, buddy);
        }
    );
}

int main()
{
    int a0, a1, a2, a3, a4;
    //atom_value calc = atom("calc");

    cout << "You have started this 'Simple Function Evaluator'" << endl;
    cout << "Enter 5 Integer-numbers with whitespaces in between the numbers: ";
    cin >> a0 >> a1 >> a2 >> a3 >> a4;

    // create a new actor 'functionEvaluator' and bound the 'a'-values to it
    auto evaluation_actor = spawn(functionEvaluator, a0, a1, a2, a3, a4);

    // create actor for handling the 'functionEvaluator'- actor
    spawn(print_evaluator, evaluation_actor);

    await_all_actors_done();

    shutdown();
}

