// 1. Have node and redux installed.
// 2. open terminal in the file directory
// 3. type in 'node redux-tut-node.js'

// import redux
const redux = require('redux');

// initial split states 
const initialCakeState = {
    noOfCakes: 10,
}

const initialIcecreamState = {
    noOfIcecreams: 10,
}

// define the actions' type
let BUY_CAKE = "BUY_CAKE";
let BUY_ICECREAM = "BUY_ICECREAM";

// now the action creators 
const buyCake = () => ({
    type: BUY_CAKE,
})

const buyIcecream = () => ({
    type: BUY_ICECREAM,
})

// here's how will we handle the change in states 
const reduceCakes = (state = initialCakeState, action) => {
    if (action.type === BUY_CAKE) return {
        ...state, 
        noOfCakes: state.noOfCakes - 1,
    }; else return state;
}

const reduceIcecreams = (state = initialIcecreamState, action) => {
    switch (action.type) {
        case BUY_ICECREAM: 
            return {
                ...state,
                noOfIcecreams: state.noOfIcecreams-1
            };
        default:
            return state; 
    }
}

// create the store to hold the state of both products
const rootReducer = redux.combineReducers({
    cake: reduceCakes,
    iceCream: reduceIcecreams,
})

// pass it into the store
const store = redux.createStore(rootReducer);

// now subscribe to the store 
console.log("initial state: ", store.getState());

const unsub = store.subscribe(() => console.log("Updated state: ", store.getState()));

// dispatch actions 
store.dispatch(buyCake());
store.dispatch(buyCake());
store.dispatch(buyCake());
store.dispatch(buyCake());
store.dispatch(buyIcecream());

// unsubscribe now
unsub();
store.dispatch(buyIcecream());
