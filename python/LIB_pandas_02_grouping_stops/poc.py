#!/usr/bin/env python3

from pathlib import Path

import pandas as pd


def display_group(grouped: pd.core.groupby.GroupBy, limit=3):
    counter = 0
    for name, group in grouped:
        print("===== NAME OF THIS GROUP =", name)
        print(group)
        print(f"this group is of type : {type(group)}")

        counter += 1
        if counter > limit:
            break


def my_tests():

    # cf. https://pandas.pydata.org/docs/reference/groupby.html

    print("PANDAS     PATH =", pd.__path__)
    stops_txt = Path(__file__).parent / "stops.txt"
    interesting_cols = ("stop_id", "stop_name", "stop_lat", "stop_lon", "stop_code", "parent_station")
    df = pd.read_csv(str(stops_txt), usecols=interesting_cols)
    print(df)

    # STEP 1 = discriminate parents and children :
    children = df[df["parent_station"].notna()]
    parents = df[df["parent_station"].isna()]
    assert len(children.index) + len(parents.index) == len(df.index)
    print("")
    print(f"There are {len(children.index)} children (with {len(parents.index)} parents)")
    # note : some parents don't have any children

    # STEP 2 = group children by parent_station :
    grouped = children.groupby("parent_station", group_keys=False)
    # 'group_keys=False' is needed to avoid this error later :
    # ValueError: 'parent_station' is both an index level and a column label, which is ambiguous.

    print("")
    print(f"There are {len(grouped)} groups")
    display_group(grouped)

    # STEP 3 = sort each group by stop_lat :
    # NOTE : by default, groupby keeps in each group the original order of the rows in the initial dataframe
    # here, we want to change each subgroup order
    print("")
    print("SORTING... then groupby again")
    after_sort = grouped.apply(lambda x: x.sort_values("stop_lat"))
    assert isinstance(after_sort, pd.core.frame.DataFrame)  # 'apply' merges the groups into a resulting DataFrame
    grouped2 = after_sort.groupby("parent_station")
    display_group(grouped2)
    # the groups displayed here are now ordered differently than before


my_tests()
