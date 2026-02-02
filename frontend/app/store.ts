import type { Action, ThunkAction } from "@reduxjs/toolkit";
import { configureStore } from "@reduxjs/toolkit";

// import reducers
import userDataReducer from "./_reducers/userData";
import { useDispatch, useSelector } from "react-redux";
// import reducers

export const store = configureStore({
  reducer: {
    userData: userDataReducer,
  },
});

export type AppStore = typeof store;
export type RootState = ReturnType<AppStore["getState"]>;

export type AppDispatch = AppStore["dispatch"];
export const useAppDispatch = useDispatch.withTypes<AppDispatch>;

export const useAppSelector = useSelector.withTypes<RootState>;

export type AppThunk<ThunkReturnType = void> = ThunkAction<
  ThunkReturnType,
  RootState,
  unknown,
  Action
>;
