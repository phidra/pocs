const mocked_data = {
  "type": "FeatureCollection",
  "features": [
    {
      "type": "Feature",
      "properties": {
        "id": "node1",
		"inhubs": ["node3", "node5", "node8"],
		"outhubs": ["node4", "node9", "node16"]
      },
      "geometry": {
        "type": "Point",
        "coordinates": [2.828629493713379, 48.84523130748805]
      }
    },
    {
      "type": "Feature",
      "properties": {
        "id": "node2",
		"inhubs": ["node4", "node6", "node9"],
		"outhubs": ["node3", "node8", "node15"]

      },
      "geometry": {
        "type": "Point",
        "coordinates": [2.8288869857788086, 48.842322257330316]
      }
    },
    {
      "type": "Feature",
      "properties": {
        "id": "node3",
		"inhubs": ["node5", "node7", "node10"],
		"outhubs": ["node6", "node11", "node16"]

      },
      "geometry": {
        "type": "Point",
        "coordinates": [2.830646514892578, 48.858819875691414]
      }
    },
    {
      "type": "Feature",
      "properties": {
        "id": "node4",
		"inhubs": ["node2", "node4", "node7"],
		"outhubs": ["node5", "node10", "node16"]

      },
      "geometry": {
        "type": "Point",
        "coordinates": [2.8250246047973633, 48.841107749500374]
      }
    },
    {
      "type": "Feature",
      "properties": {
        "id": "node5",
		"inhubs": ["node1", "node3", "node6"],
		"outhubs": ["node7", "node12", "node13"]

      },
      "geometry": {
        "type": "Point",
        "coordinates": [2.8222780227661133, 48.85850916871952]
      }
    },
    {
      "type": "Feature",
      "properties": {
        "id": "node6",
		"inhubs": ["node4", "node10", "node6"],
		"outhubs": ["node3", "node13", "node15"]

      },
      "geometry": {
        "type": "Point",
        "coordinates": [2.8209905624389644, 48.841983327875234]
      }
    },
    {
      "type": "Feature",
      "properties": {
        "id": "node7",
		"inhubs": ["node4", "node3", "node9"],
		"outhubs": ["node6", "node8", "node16"]

      },
      "geometry": {
        "type": "Point",
        "coordinates": [2.817514419555664, 48.84432754300857]
      }
    },
    {
      "type": "Feature",
      "properties": {
        "id": "node8",
		"inhubs": ["node3", "node5", "node11"],
		"outhubs": ["node4", "node8", "node15"]

      },
      "geometry": {
        "type": "Point",
        "coordinates": [2.8125362396240234, 48.84291537835776]
      }
    },
    {
      "type": "Feature",
      "properties": {
        "id": "node9",
		"inhubs": ["node1", "node2", "node3"],
		"outhubs": ["node14", "node15", "node16"]

      },
      "geometry": {
        "type": "Point",
        "coordinates": [2.808030128479004, 48.841531418367275]
      }
    },
    {
      "type": "Feature",
      "properties": {
        "id": "node10",
		"inhubs": ["node3", "node8", "node7"],
		"outhubs": ["node14", "node9", "node2"]

      },
      "geometry": {
        "type": "Point",
        "coordinates": [2.8094892501831055, 48.84554197276101]
      }
    },
    {
      "type": "Feature",
      "properties": {
        "id": "node11",
		"inhubs": ["node4", "node10", "node2"],
		"outhubs": ["node3", "node5", "node12"]

      },
      "geometry": {
        "type": "Point",
        "coordinates": [2.8073863983154297, 48.84703878751771]
      }
    },
    {
      "type": "Feature",
      "properties": {
        "id": "node12",
		"inhubs": ["node13", "node15", "node12"],
		"outhubs": ["node14", "node9", "node16"]

      },
      "geometry": {
        "type": "Point",
        "coordinates": [2.8026227951049805, 48.847462406212664]
      }
    },
    {
      "type": "Feature",
      "properties": {
        "id": "node13",
		"inhubs": ["node8", "node1", "node2"],
		"outhubs": ["node3", "node9", "node11"]

      },
      "geometry": {
        "type": "Point",
        "coordinates": [2.798116683959961, 48.848987403841825]
      }
    },
    {
      "type": "Feature",
      "properties": {
        "id": "node14",
		"inhubs": ["node13", "node5", "node8"],
		"outhubs": ["node4", "node2", "node16"]

      },
      "geometry": {
        "type": "Point",
        "coordinates": [2.8142099380493164, 48.82138776898278]
      }
    },
    {
      "type": "Feature",
      "properties": {
        "id": "node15",
		"inhubs": ["node13", "node15", "node17"],
		"outhubs": ["node3", "node2", "node1"]

      },
      "geometry": {
        "type": "Point",
        "coordinates": [2.8166990280151367, 48.846841097567015]
      }
    },
    {
      "type": "Feature",
      "properties": {
        "id": "node16",
		"inhubs": ["node7", "node15", "node17"],
		"outhubs": ["node4", "node9", "node6"]

      },
      "geometry": {
        "type": "Point",
        "coordinates": [2.821720123291015, 48.849072124570554]
      }
    },
    {
      "type": "Feature",
      "properties": {
        "id": "node17",
		"inhubs": ["node10", "node11", "node17"],
		"outhubs": ["node14", "node16", "node3"]

      },
      "geometry": {
        "type": "Point",
        "coordinates": [2.826955795288086, 48.84692582192718]
      }
    }
  ]
};

export {mocked_data};
