#!/usr/bin/env python3

from pathlib import Path

import pandas as pd


def step1_visualize(df: pd.DataFrame):
    print("=== STEP 1 = VISUALIZING :")
    print("")
    print("PRINTING :")
    print(df)

    print("")
    print("DESCRIBE :")
    print(df.describe())

    print("")
    print("DTYPES :")
    print(df.dtypes)
    # NdM : seems (partially) capable of infering data type

    print("")
    print("HEAD / TAIL :")
    print(df.head())
    print(df.tail())

    print("")
    print("INDEX / COLUMNS :")
    print(df.index)
    # here, the index is an integer index from 0 to 397 (excluded)
    print(df.columns)


def step2_drop_unused_columns(df: pd.DataFrame):
    print("")
    print("=== STEP 2 = DROPPING UNUSED COLUMNS :")
    print(df)
    df.drop(["stop_url", "stop_desc"], axis="columns", inplace=True)
    print(df)


def step3_count_parent_stations(df: pd.DataFrame):
    # TIP : counting number of lines in DataFrame is done with :  len(df.index)
    print("")
    print("=== STEP 3 = COUNTING PARENT STATIONS:")
    lines_without_parent_stations = df[df["parent_station"].isna()]
    nb_lines_without_parent_stations = len(lines_without_parent_stations.index)

    lines_with_parent_stations = df[df["parent_station"].notna()]
    nb_lines_with_parent_stations = len(lines_with_parent_stations.index)

    assert nb_lines_without_parent_stations + nb_lines_with_parent_stations == len(df.index)

    print(lines_without_parent_stations)
    print(f"There are {nb_lines_without_parent_stations} lines without a parent station")
    print(f"There are {nb_lines_with_parent_stations} lines without a parent station")
    print(f"Sum = {nb_lines_with_parent_stations+nb_lines_without_parent_stations} ; total nb lines = {len(df.index)}")


def step4_check_parent_stations_fields(df: pd.DataFrame):
    print("")
    print("=== STEP 4 = CHECKING THAT ALL PARENT STATIONS HAVE location_type==1 :")
    parent_stations = df[df["parent_station"].isna()]
    loc_type_is_1 = df[df["location_type"] == 1]
    assert parent_stations.equals(loc_type_is_1)
    print("I can confirm that all parent stations have location_type==1 (and vice-versa)")


def step5_get_bbox(df: pd.DataFrame):
    print("")
    print("=== STEP 5 = GETTING BBOX OF LOCATIONS :")
    min_lng = df["stop_lon"].min(skipna=True)
    max_lng = df["stop_lon"].max(skipna=True)
    min_lat = df["stop_lat"].min(skipna=True)
    max_lat = df["stop_lat"].max(skipna=True)
    print(f"BBOX is ({min_lng},{min_lat})  ->  ({max_lng},{max_lat})")

    bbox_geojson = """
{{
  "type": "FeatureCollection",
  "features": [
    {{
      "type": "Feature",
      "properties": {{}},
      "geometry": {{
        "type": "Polygon",
        "coordinates": [
          [
            [{min_lng}, {min_lat}],
            [{max_lng}, {min_lat}],
            [{max_lng}, {max_lat}],
            [{min_lng}, {max_lat}],
            [{min_lng}, {min_lat}]
          ]
        ]
      }}
    }}
  ]
}}
    """.format(min_lng=min_lng, max_lng=max_lng, min_lat=min_lat, max_lat=max_lat)
    print(f"BBOX geojson = {bbox_geojson}")


def my_tests():
    print("PANDAS     PATH =", pd.__path__)
    stops_txt = Path(__file__).parent / "stops.txt"
    df = pd.read_csv(str(stops_txt))

    step1_visualize(df)
    step2_drop_unused_columns(df)
    step3_count_parent_stations(df)
    step4_check_parent_stations_fields(df)
    step5_get_bbox(df)


my_tests()
