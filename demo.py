import argparse
import logging
from wikipedia_module import Network
from tqdm import tqdm
from collections import defaultdict
import matplotlib.pyplot as plt

logger = logging.getLogger(__name__)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(prog="Demo program for wikipedia simulation", add_help=True)
    parser.add_argument("--num-editors", type=int, default=500, help = "Number of Editors in the network")
    parser.add_argument("--num-articles", type=int, default=500, help = "Number of Articles in the network")
    parser.add_argument("--iteration", type=int, default=1000, help = "Number of iterations.")
    parser.add_argument("--dimensions", type = int, default=20, help="Number of knowledge dimensions")
    parser.add_argument("--verbose", action="store_true")
    args = parser.parse_args()

    handler = logging.StreamHandler()
    logger.addHandler(handler)
    logger.setLevel(logging.DEBUG if args.verbose else logging.INFO)

    logger.info("Starting simulation")
    net = Network(args.dimensions, args.num_editors, args.num_articles)
    result = defaultdict(list)

    for _ in tqdm(range(args.iteration)):
        net.create_connections()
        logger.debug("Created connections")

        for k, v in net.adjacency.items():
            result[k].append(v)

        net.update_articles()
        logger.debug("Updated articles")

        net.clear_connections()
        logger.debug("Connections cleared")

    result = {k: set(v) for k, v in result.items()}

    plt.hist([len(v) for v in result.values()])
    plt.savefig("simulation_result.png", dpi=300)
    plt.close()
    

