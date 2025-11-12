import streamlit as st
import numpy as np
import pandas as pd

map_data = pd.DataFrame(
    np.random.randn(1000, 2) / [50, 50] + [39.87, 116.288],
    columns=['lat', 'lon'])

st.map(map_data)
st.dataframe(map_data)
