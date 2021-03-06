Botan Crypto Library
========================================


Botan is a C++11 library for crypto and TLS released under the permissive
2-clause BSD license (see ``doc/license.txt`` for the specifics).

For all the details on building the library read ``doc/manual/building.rst``,
but basically::

  $ configure.py --help
  $ configure.py [probably some options]
  $ make
  $ ./botan-test
  # test output
  $ ./botan
  # shows available commands
  $ make install

You can file bugs at https://github.com/randombit/botan/issues/
or by sending a report to the `botan-devel mailing list
<http://lists.randombit.net/mailman/listinfo/botan-devel/>`_

The `github wiki <https://github.com/randombit/botan/wiki>`_
is also available as a resource.

CI Status
----------------------------------------

.. image:: https://travis-ci.org/randombit/botan.svg?branch=net.randombit.botan
    :target: https://travis-ci.org/randombit/botan

.. image:: https://ci.appveyor.com/api/projects/status/n9f94dljd03j2lce?svg=true
    :target: https://ci.appveyor.com/project/randombit/botan

.. image:: https://coveralls.io/repos/randombit/botan/badge.svg
    :target: https://coveralls.io/r/randombit/botan

Kullo GmbH hosts a CI building botan on Linux, OS X, and Windows at
https://botan-ci.kullo.net/
