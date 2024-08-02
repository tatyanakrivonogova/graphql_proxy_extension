createPerson='INSERT INTO graphql_proxy.Person(id, name) VALUES($1, $2);';

updatePerson='UPDATE graphql_proxy.Person SET name = $1 WHERE id = $2;';

deletePerson='DELETE FROM graphql_proxy.Person WHERE id = $1;';
