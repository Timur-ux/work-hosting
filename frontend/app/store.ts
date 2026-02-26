import type { Action, ThunkAction } from "@reduxjs/toolkit";
import { configureStore } from "@reduxjs/toolkit";

// import reducers
import profileState from "./_reducers/profile";
import { useDispatch, useSelector } from "react-redux";
// import reducers

export const makeStore = () =>
  configureStore({
    reducer: {
      profile: profileState,
    },
  });

export type AppStore = ReturnType<typeof makeStore>;
export type RootState = ReturnType<AppStore['getState']>;

export type AppDispatch = AppStore["dispatch"];

export type AppThunk<ThunkReturnType = void> = ThunkAction<
  ThunkReturnType,
  RootState,
  unknown,
  Action
>;
